#include <setjmp.h>
/* XH: stddef.h */
/* XH: sun3_error.h */

struct sun3_errors {
	struct sun3_error * handler;
	jmp_buf    environment;
	size_t     unsupported_aout_magic_value;
	size_t     not_sun3_image;
	size_t     not_enough_virtual_memory;
	size_t     unimplemented_instruction;
	size_t     illegal_instruction;
	size_t     address_error;
	size_t     bus_error;
	size_t     unimplemented_trap;
	size_t     trap_exception;
	size_t     privilige_violation;
	size_t     unimplemented_system_call;
};

typedef struct sun3_errors sun3_errors;

void sun3_errors_open(sun3_errors *, struct sun3_error *);

int sun3_errors_handler(sun3_errors *);

void sun3_errors_normal_exit(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_bus_error(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_address_error(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_privilige_violation(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_unimplemented_instruction(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_illegal_instruction(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_unimplemented_trap(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_unimplemented_system_call(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_trap_exception(sun3_errors *) __attribute__ ((__noreturn__));

void sun3_errors_close(sun3_errors *);
