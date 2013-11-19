#include <stddef.h>
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_memory_write.h"
#include "sun3_memory_io.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_memory_io.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_trace.h"
#include "sun3_trap.h"

static void sun3_trap_exit(sun3 *vm)
{
	sun3_errors_normal_exit(vm->errors);
}

static void sun3_trap_read(sun3 *vm)
{
	sun3_memory_address sp = sun3_address_regs_read_unsigned_long(&vm->address_regs, 7);
	sun3_memory_address fid_addr = sp+sizeof(sun3_unsigned_long);
	sun3_unsigned_long fid = sun3_memory_read_unsigned_long(&vm->memory, fid_addr);
	sun3_memory_address str_addr = fid_addr+sizeof(sun3_unsigned_long);
	sun3_unsigned_long str = sun3_memory_read_unsigned_long(&vm->memory, str_addr);
	sun3_memory_address len_addr = str_addr+sizeof(sun3_unsigned_long);
	sun3_unsigned_long len = sun3_memory_read_unsigned_long(&vm->memory, len_addr);
	int nread = sun3_memory_io_read(fid, &vm->memory, str, len);

	sun3_sr_set_c(vm->sr, 0);
	sun3_data_regs_write_signed_long(&vm->data_regs, 0, nread);
}

static void sun3_trap_write(sun3 *vm)
{
	sun3_memory_address sp = sun3_address_regs_read_unsigned_long(&vm->address_regs, 7);
	sun3_memory_address sink_addr = sp+sizeof(sun3_unsigned_long);
	sun3_unsigned_long sink = sun3_memory_read_unsigned_long(&vm->memory, sink_addr);
	sun3_memory_address str_addr = sink_addr+sizeof(sun3_unsigned_long);
	sun3_unsigned_long str = sun3_memory_read_unsigned_long(&vm->memory, str_addr);
	sun3_memory_address len_addr = str_addr+sizeof(sun3_unsigned_long);
	sun3_unsigned_long len = sun3_memory_read_unsigned_long(&vm->memory, len_addr);
	int nwritten = sun3_memory_io_write(sink, &vm->memory, str, len);

	nwritten -= len;
	if (nwritten) {
		sun3_sr_set_c(vm->sr, 0);
	} else {
		sun3_sr_set_c(vm->sr, 1);
	}
	sun3_data_regs_write_signed_long(&vm->data_regs, 0, nwritten);
}

static void sun3_trap_open(sun3 *vm)
{
	sun3_memory_address sp = sun3_address_regs_read_unsigned_long(&vm->address_regs, 7);
	sun3_memory_address name_addr = sp+sizeof(sun3_unsigned_long);
	/*
	 * XXX: ugly hack alert, the following will not worth with all
	 * memory implementations
	 */
	const char *name = (const char *)&vm->memory.data[sun3_memory_read_unsigned_long(&vm->memory, name_addr)];
	int fid;

	if (!sun3_memory_io_open(name, &fid)) {
		sun3_sr_set_c(vm->sr, 1);
	} else {
		sun3_sr_set_c(vm->sr, 0);
	}
	sun3_data_regs_write_signed_long(&vm->data_regs, 0, fid);
}

static void sun3_trap_close(sun3 *vm)
{
	sun3_memory_address sp = sun3_address_regs_read_unsigned_long(&vm->address_regs, 7);
	sun3_memory_address fid_addr = sp+sizeof(sun3_unsigned_long);
	sun3_unsigned_long fid = sun3_memory_read_unsigned_long(&vm->memory, fid_addr);

	if (!sun3_memory_io_close(fid)) {
		sun3_sr_set_c(vm->sr, 1);
	} else {
		sun3_sr_set_c(vm->sr, 0);
	}
}

static void sun3_trap_unix(struct sun3 *vm)
{
	switch (sun3_data_regs_read_unsigned_long(&vm->data_regs, 0)) {
	case 1:
		sun3_trap_exit(vm);
		break;
	case 3:
		sun3_trap_read(vm);
		break;
	case 4:
		sun3_trap_write(vm);
		break;
	case 5:
		sun3_trap_open(vm);
		break;
	case 6:
		sun3_trap_close(vm);
		break;
	default:
		sun3_errors_unimplemented_system_call(vm->errors);
	}
}

/*
 * The "hypervisor" here is just the sun3 emulator itself.
 */
static void sun3_trap_hypervisor(struct sun3 *vm)
{
	switch (sun3_data_regs_read_unsigned_long(&vm->data_regs, 0)) {
	case 1:			/* sun3_vma_start */
		sun3_data_regs_write_unsigned_long(&vm->data_regs, 0, vm->memory.read_start);
		break;
	case 2:			/* sun3_vma_end */
		sun3_data_regs_write_unsigned_long(&vm->data_regs, 0, vm->memory.end);
		break;
	case 3:			/* sun3_vma_rw_start */
		sun3_data_regs_write_unsigned_long(&vm->data_regs, 0, vm->memory.write_start);
		break;
	default:
		sun3_errors_unimplemented_system_call(vm->errors);
	}
}


void sun3_instr_trap(sun3 *vm)
{
	sun3_memory_address addr = sun3_instr_trap_addr(vm->pci);

	sun3_trace_instr(vm, "trap #%"sun3_memory_address_fmtx"", (int)addr);
	if (addr == 0) {
		sun3_trap_unix(vm);
	} else if (addr == 8) {
		sun3_trap_hypervisor(vm);
	} else {
		sun3_errors_unimplemented_trap(vm->errors);
	}
}
