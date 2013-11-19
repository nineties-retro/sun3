#include <stddef.h>
#include <assert.h>
#include "sun3_machine_types.h"
#include "sun3_data_regs.h"


void sun3_data_regs_write_unsigned_byte(sun3_data_regs     *regs,
					size_t              reg,
					sun3_unsigned_byte  value)
{
	assert(reg < sun3_data_regs_nregs);
	regs->regs[reg]= (regs->regs[reg]&~0xff)|value;
}



void sun3_data_regs_write_unsigned_word(sun3_data_regs     *regs,
					size_t              reg,
					sun3_unsigned_word  value)
{
	assert(reg < sun3_data_regs_nregs);
	regs->regs[reg]= (regs->regs[reg]&~0xffff)|value;
}


void sun3_data_regs_write_unsigned_long(sun3_data_regs     *regs,
					size_t              reg,
					sun3_unsigned_long  value)
{
	assert(reg < sun3_data_regs_nregs);
	regs->regs[reg]= value;
}



void sun3_data_regs_write_signed_byte(sun3_data_regs   *regs,
				      size_t            reg,
				      sun3_signed_byte  value)
{
	assert(reg < sun3_data_regs_nregs);
	regs->regs[reg]= (regs->regs[reg]&~0xff)|value;
}



void sun3_data_regs_write_signed_word(sun3_data_regs   *regs,
				      size_t            reg,
				      sun3_signed_word  value)
{
	assert(reg < sun3_data_regs_nregs);
	regs->regs[reg]= (regs->regs[reg]&~0xffff)|value;
}


void sun3_data_regs_write_signed_long(sun3_data_regs   *regs,
				      size_t            reg,
				      sun3_signed_long  value)
{
	assert(reg < sun3_data_regs_nregs);
	regs->regs[reg]= value;
}




sun3_unsigned_byte sun3_data_regs_read_unsigned_byte(sun3_data_regs * regs, size_t reg)
{
	assert(reg < sun3_data_regs_nregs);
	return (sun3_unsigned_byte)regs->regs[reg];
}


sun3_unsigned_word sun3_data_regs_read_unsigned_word(sun3_data_regs * regs, size_t reg)
{
	assert(reg < sun3_data_regs_nregs);
	return (sun3_unsigned_word)regs->regs[reg];
}


sun3_unsigned_long sun3_data_regs_read_unsigned_long(sun3_data_regs * regs, size_t reg)
{
	assert(reg < sun3_data_regs_nregs);
	return regs->regs[reg];
}


sun3_signed_byte sun3_data_regs_read_signed_byte(sun3_data_regs * regs, size_t reg)
{
	return (sun3_signed_byte)sun3_data_regs_read_unsigned_byte(regs, reg);
}


sun3_signed_word sun3_data_regs_read_signed_word(sun3_data_regs * regs, size_t reg)
{
	return (sun3_signed_word)sun3_data_regs_read_unsigned_word(regs, reg);
}


sun3_signed_long sun3_data_regs_read_signed_long(sun3_data_regs * regs, size_t reg)
{
	return (sun3_signed_long)sun3_data_regs_read_unsigned_long(regs, reg);
}
