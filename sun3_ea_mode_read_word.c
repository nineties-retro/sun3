#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instruction.h"
#include "sun3_instr.h"
#include "sun3_ea_mode_read_word.h"


void sun3_ea_mode_read_word(sun3 *vm, sun3_word_instruction *i)
{
	switch(vm->ea_mode) {
	case sun3_instr_mode_DRD:
		if (i->legal_execute_modes&sun3_ea_mode_drd) {
			i->src= sun3_data_regs_read_unsigned_word(&vm->data_regs, vm->reg_num);
		} else {
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	case sun3_instr_mode_ARD:
		if (i->legal_execute_modes&sun3_ea_mode_ard) {
			i->src= sun3_address_regs_read_unsigned_word(&vm->address_regs, vm->reg_num);
		} else {
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	case sun3_instr_mode_ARI:
		if (i->legal_execute_modes&sun3_ea_mode_ari) {
			sun3_memory_address addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
			i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
		} else {
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	case sun3_instr_mode_ARIPI:
		if (i->legal_execute_modes&sun3_ea_mode_aripi) {
			sun3_memory_address addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
			i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
			sun3_address_regs_write_unsigned_long(&vm->address_regs, vm->reg_num, addr+sizeof(sun3_unsigned_word));
		} else {
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	case sun3_instr_mode_ARIPD:
		if (i->legal_execute_modes&sun3_ea_mode_aripd) {
			sun3_memory_address addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num)-sizeof(sun3_unsigned_word);
			sun3_address_regs_write_unsigned_long(&vm->address_regs, vm->reg_num, addr+sizeof(sun3_unsigned_long));
			addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
			i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
		} else {
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	case sun3_instr_mode_ARID:
		if (i->legal_execute_modes&sun3_ea_mode_arid) {
			sun3_memory_address addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
			sun3_signed_long disp= (sun3_signed_long)sun3_memory_read_signed_word(&vm->memory, vm->pc);
			i->src= sun3_memory_read_unsigned_word(&vm->memory, addr+disp);
			vm->pc += sizeof(sun3_signed_word);
		} else {
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	case sun3_instr_mode_ARII:
		if (i->legal_execute_modes&sun3_ea_mode_arii) {
			sun3_unsigned_word  npci= sun3_memory_read_signed_word(&vm->memory, vm->pc);
			sun3_memory_address addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num) + (sun3_signed_byte)((sun3_unsigned_byte)npci);
			unsigned int       reg= npci>>12;
			sun3_unsigned_long  disp= (reg&0x8)
				? sun3_address_regs_read_unsigned_long(&vm->address_regs, reg&0x7)
				: sun3_data_regs_read_unsigned_long(&vm->data_regs, reg);
			addr += (npci&(1<<11))
				? (sun3_signed_long)disp
				: (sun3_signed_long)((sun3_unsigned_word)disp);
			i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
			vm->pc += sizeof(sun3_signed_word);
		} else {
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	case sun3_instr_mode_EXTD:
		switch(vm->reg_num) {
		case sun3_instr_mode_ASA:
			if (i->legal_execute_modes&sun3_ea_mode_asa) {
				sun3_memory_address addr= (sun3_memory_address)((sun3_signed_long)sun3_memory_read_signed_word(&vm->memory, vm->pc));
				i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
				vm->pc += sizeof(sun3_unsigned_word);
			} else {
				sun3_errors_illegal_instruction(vm->errors);
			}
			break;
		case sun3_instr_mode_ALA:
			if (i->legal_execute_modes&sun3_ea_mode_ala) {
				sun3_memory_address addr= (sun3_memory_address)sun3_memory_read_unsigned_long(&vm->memory, vm->pc);
				i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
				vm->pc += sizeof(sun3_unsigned_long);
			} else {
				sun3_errors_illegal_instruction(vm->errors);
			}
			break;
		case sun3_instr_mode_PCD:
			if (i->legal_execute_modes&sun3_ea_mode_pcd) {
				sun3_memory_address addr= vm->pc + sun3_memory_read_signed_word(&vm->memory, vm->pc);
				i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
				vm->pc += sizeof(sun3_signed_word);
			} else {
				sun3_errors_illegal_instruction(vm->errors);
			}
			break;
		case sun3_instr_mode_PCI:
			if (i->legal_execute_modes&sun3_ea_mode_pci) {
				sun3_unsigned_word  npci= sun3_memory_read_signed_word(&vm->memory, vm->pc);
				unsigned int       reg= npci>>12;
				sun3_unsigned_long  disp= (reg&0x8)
					? sun3_address_regs_read_unsigned_long(&vm->address_regs, reg&0x7)
					: sun3_data_regs_read_unsigned_long(&vm->data_regs, reg);
				sun3_memory_address addr= vm->pc + (sun3_signed_byte)((sun3_unsigned_byte)npci);
				addr += (npci&(1<<11))
					? (sun3_signed_long)disp
					: (sun3_signed_long)((sun3_unsigned_word)disp);
				i->src= sun3_memory_read_unsigned_word(&vm->memory, addr);
				vm->pc += sizeof(sun3_signed_word);
			} else {
				sun3_errors_illegal_instruction(vm->errors);
			}
			break;
		case sun3_instr_mode_ID:
			if (i->legal_execute_modes&sun3_ea_mode_id) {
				i->src= sun3_memory_read_signed_word(&vm->memory, vm->pc);
				vm->pc += sizeof(sun3_signed_word);
			} else {
				sun3_errors_illegal_instruction(vm->errors);
			}
			break;
		default:
			sun3_errors_illegal_instruction(vm->errors);
			break;
		}
		break;
	default:
		sun3_errors_unimplemented_instruction(vm->errors);
	}
}
