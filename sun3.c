/* A simple MC68000 instruction set emulator */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_data_regs.h"
#include "sun3_data_regs_dump.h"
#include "sun3_address_regs.h"
#include "sun3_address_regs_dump.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_aout_read.h"
#include "sun3_run.h"
#include "sun3_unused_param.h"

static char * program_name;
static size_t usage_error;


static void
usage(sun3_error * error_handler)
{
	sun3_error_register_code(error_handler, usage_error);
	sun3_error_register_message(error_handler, "[ -vn ] [ -wn ] image-file");
}


static void
display_error(sun3_errors * errors)
{
	int error_code= sun3_error_code(errors->handler);
	switch(error_code) {
	case sun3_error_file_io:
		fprintf(stderr,
			"%s: could not %s \"%s\"\n",
			program_name,
			sun3_error_arg(errors->handler, 1),
			sun3_error_arg(errors->handler, 0));
		break;

	case sun3_error_memory:
		break;

	case sun3_error_none:
		return;
		
	default:
		if ((size_t)error_code == usage_error) {
			fprintf(stderr, "%s: %s\n", program_name, sun3_error_arg(errors->handler, 0));
			return;
		} else if ((size_t)error_code == errors->unsupported_aout_magic_value) {
			fprintf(stderr, "%s: \"%s\" does not contain an NMAGIC, OMAGIC or ZMAGIC a.out image\n", program_name, sun3_error_arg(errors->handler, 0));
			return;
		} else if ((size_t)error_code == errors->not_sun3_image) {
			fprintf(stderr, "%s: \"%s\" does not contain a SUN3 a.out image\n", program_name, sun3_error_arg(errors->handler, 0));
			return;
		} else if ((size_t)error_code == errors->not_enough_virtual_memory) {
			fprintf(stderr, "%s: not enough virtual memory\n", program_name);
			return;
		} else if ((size_t)error_code == errors->address_error) {
			fprintf(stderr, "%s: address_error\n", program_name);
			return;
		} else if ((size_t)error_code == errors->unimplemented_instruction) {
			fprintf(stderr, "%s: unimplemented_instruction\n", program_name);
			return;
		} else if ((size_t)error_code == errors->illegal_instruction) {
			fprintf(stderr, "%s: illegal_instruction\n", program_name);
			return;
		} else if ((size_t)error_code == errors->bus_error) {
			fprintf(stderr, "%s: bus_error\n", program_name);
			return;
		} else if ((size_t)error_code == errors->unimplemented_trap) {
			fprintf(stderr, "%s: unimplemented_trap\n", program_name);
			return;
		} else if ((size_t)error_code == errors->trap_exception) {
			fprintf(stderr, "%s: trap_exception\n", program_name);
			return;
		} else if ((size_t)error_code == errors->privilige_violation) {
			fprintf(stderr, "%s: unimplemented_instruction\n", program_name);
			return;
		} else if ((size_t)error_code == errors->unimplemented_system_call) {
			fprintf(stderr, "%s: unimplemented_system_call\n", program_name);
			return;
		}
		break;
	}
	fprintf(stderr, "%s: system error: %s\n", program_name, sun3_error_system_message(errors->handler));
}


static int
sun3_init(sun3 * vm, sun3_errors * errors)
{
	sun3_memory_init(&vm->memory, errors);
	sun3_memory_set_size(&vm->memory, 256*1024);
	vm->errors= errors;
	return 1;
}



static int
sun3_open(sun3 *vm)
{
	if (!sun3_memory_open(&vm->memory))
		goto could_not_open_memory;
	if (!sun3_aout_read(vm->image_file_name, &vm->memory, vm->errors, &vm->start_address))
		goto could_not_read_aout_image;
	return 1;
	
could_not_read_aout_image:
	sun3_memory_close(&vm->memory);
could_not_open_memory:
	return 0;
}



static int
sun3_close(sun3 *vm)
{
	sun3_memory_close(&vm->memory);
	return 1;
}



static int
process_arguments(int argc, char **argv, sun3 * vm)
{
	program_name= argv[0];
	usage_error= sun3_error_allocate_code(vm->errors->handler);
	while (++argv, --argc != 0 && argv[0][0] == '-') {
		switch(argv[0][1]) {
		case 'v':
		{
			size_t image_size= atol(&argv[0][2]);
			sun3_memory_set_size(&vm->memory, image_size);
		}
		break;
		case 'h':
		default:
			usage(vm->errors->handler);
			return 0;
		}
	}
	if (argc != 1) {
		usage(vm->errors->handler);
		return 0;
	}
	vm->image_file_name= argv[0];
	return 1;
}



int main(int argc, char ** argv)
{
	sun3        vm;
	sun3_errors errors;
	sun3_error   error_handler;
	sun3_error_open(&error_handler);
	sun3_errors_open(&errors, &error_handler);
	
	if (!sun3_init(&vm, &errors))
		goto could_not_init_vm;
	
	if (!process_arguments(argc, argv, &vm))
		goto could_not_process_arguments;

	switch(setjmp(vm.errors->environment)) {
	case 1:
		goto normal_exit;
	case -1:
		goto abnormal_exit;
	default:
		break;
	}

	if (!sun3_open(&vm))
		goto could_not_open_vm;

	if (!sun3_run(&vm)) 
		goto could_not_run_vm;

normal_exit:
	if (!sun3_close(&vm))
		goto could_not_close_vm;

	sun3_errors_close(&errors);
	sun3_error_close(&error_handler);
	return EXIT_SUCCESS;

could_not_run_vm:
abnormal_exit:
	fprintf(stderr, "PC %08"sun3_memory_address_fmtx"\n", vm.pc);
	sun3_data_regs_dump(&vm.data_regs, stderr);
	sun3_address_regs_dump(&vm.address_regs, stderr);
	(void)sun3_close(&vm);
could_not_close_vm:
could_not_open_vm:
could_not_process_arguments:
could_not_init_vm:
	display_error(&errors);
	sun3_errors_close(&errors);
	sun3_error_close(&error_handler);
	return EXIT_FAILURE;
}
