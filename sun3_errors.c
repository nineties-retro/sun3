#include <stddef.h>
#include <setjmp.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_unused_param.h"

void sun3_errors_open(struct sun3_errors *errors, struct sun3_error *error_handler)
{
	errors->handler= error_handler;
	errors->unsupported_aout_magic_value= sun3_error_allocate_code(error_handler);
	errors->not_sun3_image= sun3_error_allocate_code(error_handler);
	errors->not_enough_virtual_memory= sun3_error_allocate_code(error_handler);
	errors->unimplemented_instruction= sun3_error_allocate_code(error_handler);
	errors->illegal_instruction= sun3_error_allocate_code(error_handler);
	errors->address_error= sun3_error_allocate_code(error_handler);
	errors->bus_error= sun3_error_allocate_code(error_handler);
	errors->unimplemented_trap= sun3_error_allocate_code(error_handler);
	errors->trap_exception= sun3_error_allocate_code(error_handler);
	errors->privilige_violation= sun3_error_allocate_code(error_handler);
	errors->unimplemented_system_call= sun3_error_allocate_code(error_handler);
}


void sun3_errors_normal_exit(sun3_errors * errors)
{
	sun3_error_register_code(errors->handler, errors->bus_error);
	longjmp(errors->environment, 1);
}


void sun3_errors_bus_error(sun3_errors * errors)
{
	sun3_error_register_code(errors->handler, errors->bus_error);
	longjmp(errors->environment, -1);
}



void sun3_errors_address_error(sun3_errors *errors)
{
	sun3_error_register_code(errors->handler, errors->address_error);
	longjmp(errors->environment, -1);
}



void sun3_errors_privilige_violation(sun3_errors *errors)
{
	sun3_error_register_code(errors->handler, errors->privilige_violation);
	longjmp(errors->environment, -1);
}



void sun3_errors_unimplemented_instruction(sun3_errors *errors)
{
	sun3_error_register_code(errors->handler, errors->unimplemented_instruction);
	longjmp(errors->environment, -1);
}



void sun3_errors_illegal_instruction(sun3_errors *errors)
{
	sun3_error_register_code(errors->handler, errors->illegal_instruction);
	longjmp(errors->environment, -1);
}



void sun3_errors_unimplemented_trap(sun3_errors *errors)
{
	sun3_error_register_code(errors->handler, errors->unimplemented_trap);
	longjmp(errors->environment, -1);
}


void sun3_errors_trap_exception(sun3_errors *errors)
{
	sun3_error_register_code(errors->handler, errors->trap_exception);
	longjmp(errors->environment, -1);
}


void sun3_errors_unimplemented_system_call(sun3_errors *errors)
{
	sun3_error_register_code(errors->handler, errors->unimplemented_system_call);
	longjmp(errors->environment, -1);
}



void sun3_errors_close(sun3_errors *errors sun3_unused_param)
{
}
