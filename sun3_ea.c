#include <assert.h>
#include <stddef.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_ea.h"

sun3_memory_address sun3_ea(sun3 *vm)
{
	switch(vm->ea_mode) {
	case sun3_instr_mode_ARI:
		return sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
	case sun3_instr_mode_ARID:
	{
		sun3_memory_address Creg= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
		sun3_signed_long disp= (sun3_signed_long)sun3_memory_read_signed_word(&vm->memory, vm->pc);
		sun3_memory_address addr= Creg+disp;
		vm->pc += sizeof(sun3_signed_word);
		return addr;
	}
	case sun3_instr_mode_ARII:
	{
		sun3_unsigned_word  npci= sun3_memory_read_signed_word(&vm->memory, vm->pc);
		sun3_memory_address addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num) + (sun3_signed_byte)((sun3_unsigned_byte)npci);
		unsigned int       reg= npci>>12;
		sun3_unsigned_long  disp= (reg&0x8)
			? sun3_address_regs_read_unsigned_long(&vm->address_regs, reg&0x7)
			: sun3_data_regs_read_unsigned_long(&vm->data_regs, reg);
		addr += (npci&(1<<11))
			? (sun3_signed_long)disp
			: (sun3_signed_long)((sun3_unsigned_word)disp);
		return addr;
	}
	case sun3_instr_mode_EXTD:
		switch(vm->reg_num) {
		case sun3_instr_mode_ASA:
		{
			sun3_memory_address addr= (sun3_memory_address)((sun3_signed_long)sun3_memory_read_signed_word(&vm->memory, vm->pc));
			vm->pc += sizeof(sun3_signed_word);
			return addr;
		}
		case sun3_instr_mode_ALA:
		{
			sun3_memory_address addr= (sun3_memory_address)sun3_memory_read_unsigned_long(&vm->memory, vm->pc);
			vm->pc += sizeof(sun3_unsigned_long);
			return addr;
		}
		case sun3_instr_mode_PCD:
		{
			sun3_memory_address addr= vm->pc + sun3_memory_read_signed_word(&vm->memory, vm->pc);
			vm->pc += sizeof(sun3_signed_word);
			return addr;
		}
		case sun3_instr_mode_PCI:
		{
			sun3_unsigned_word  npci= sun3_memory_read_signed_word(&vm->memory, vm->pc);
			unsigned int       reg= npci>>12;
			sun3_unsigned_long  disp= (reg&0x8)
				? sun3_address_regs_read_unsigned_long(&vm->address_regs, reg&0x7)
				: sun3_data_regs_read_unsigned_long(&vm->data_regs, reg);
			sun3_memory_address addr= vm->pc + (sun3_signed_byte)((sun3_unsigned_byte)npci);
			addr += (npci&(1<<11))
				? (sun3_signed_long)disp
				: (sun3_signed_long)((sun3_unsigned_word)disp);
			vm->pc += sizeof(sun3_signed_word);
			return addr;
		}
		default:
			sun3_errors_illegal_instruction(vm->errors);
			break;
		}
		break;
	default:
		sun3_errors_illegal_instruction(vm->errors);
		break;
	}
}
