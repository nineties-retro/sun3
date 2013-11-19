#include <stddef.h>
#include <assert.h>
#include "sun3_machine_types.h"
#include "sun3_address_regs.h"
#include "sun3_unused_param.h"

void sun3_address_regs_write_signed_byte(sun3_address_regs *regs  sun3_unused_param,
					 size_t             reg   sun3_unused_param,
					 sun3_signed_byte   value sun3_unused_param)
{
	/* Not legal, but included to make automatic generation of .c files easier */
	assert(0);
}



void sun3_address_regs_write_signed_word(sun3_address_regs *regs,
					size_t              reg,
					sun3_signed_word    value)
{
	assert(reg < sun3_address_regs_nregs);
	regs->regs[reg]= (sun3_signed_long)value;
}


void sun3_address_regs_write_signed_long(sun3_address_regs *regs,
					size_t              reg,
					sun3_signed_long    value)
{
	assert(reg < sun3_address_regs_nregs);
	regs->regs[reg]= value;
}



void sun3_address_regs_write_unsigned_byte(sun3_address_regs *regs   sun3_unused_param,
					  size_t              reg    sun3_unused_param,
					  sun3_unsigned_byte  value  sun3_unused_param)
{
	/* Not legal, but included to make automatic generation of .c files easier */
	assert(0);
}



void sun3_address_regs_write_unsigned_word(sun3_address_regs *regs,
					  size_t              reg,
					  sun3_unsigned_word  value)
{
	assert(reg < sun3_address_regs_nregs);
	regs->regs[reg]= value;
}



void sun3_address_regs_write_unsigned_long (sun3_address_regs *regs,
					   size_t              reg,
					   sun3_unsigned_long  value)
{
	assert(reg < sun3_address_regs_nregs);
	regs->regs[reg]= value;
}



sun3_unsigned_byte sun3_address_regs_read_unsigned_byte(sun3_address_regs *regs  sun3_unused_param,
						      size_t               reg   sun3_unused_param)
{
	/* Not legal, but included to make automatic generation of .c files easier */
	assert(0);
	return 0;
}


sun3_unsigned_word sun3_address_regs_read_unsigned_word(sun3_address_regs *regs, size_t reg)
{
	assert(reg < sun3_address_regs_nregs);
	return (sun3_unsigned_word)regs->regs[reg];
}



sun3_unsigned_long sun3_address_regs_read_unsigned_long(sun3_address_regs *regs, size_t reg)
{
	assert(reg < sun3_address_regs_nregs);
	return regs->regs[reg];
}


sun3_signed_byte sun3_address_regs_read_signed_byte(sun3_address_regs *regs  sun3_unused_param,
						  size_t               reg   sun3_unused_param)
{
	/* Not legal, but included to make automatic generation of .c files easier */
	assert(0);
	return 0;
}


sun3_signed_word sun3_address_regs_read_signed_word(sun3_address_regs *regs, size_t reg)
{
	return (sun3_signed_word)sun3_address_regs_read_unsigned_word(regs, reg);
}



sun3_signed_long sun3_address_regs_read_signed_long(sun3_address_regs *regs, size_t reg)
{
	return (sun3_signed_long)sun3_address_regs_read_unsigned_long(regs, reg);
}
