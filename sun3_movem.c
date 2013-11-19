#include <stddef.h>
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_memory_write.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_movem.h"

void sun3_movem_reg_to_mem_long(sun3 *vm)
{
	switch (vm->ea_mode) {
	case sun3_instr_mode_ARI:
		sun3_errors_unimplemented_instruction(vm->errors);
		break;
	case sun3_instr_mode_ARIPI:
		sun3_errors_illegal_instruction(vm->errors);
		break;
	case sun3_instr_mode_ARIPD:
	{
		sun3_unsigned_word reg_mask= sun3_memory_read_unsigned_word(&vm->memory, vm->pc);
		sun3_memory_address sp= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
		unsigned int reg_num;
		for (reg_num= 8; reg_num != 0; reg_num-=1, reg_mask>>=1) {
			if (reg_mask&0x1) {
				sun3_unsigned_long reg= sun3_address_regs_read_unsigned_long(&vm->address_regs, reg_num-1);
				sp -= sizeof(sun3_unsigned_long);
				sun3_memory_write_long(&vm->memory, sp, reg);
			}
		}
		for (reg_num= 8; reg_num != 0; reg_num-=1, reg_mask>>=1) {
			if (reg_mask&0x1) {
				sun3_unsigned_long reg= sun3_data_regs_read_unsigned_long(&vm->data_regs, reg_num-1);
				sp -= sizeof(sun3_unsigned_long);
				sun3_memory_write_long(&vm->memory, sp, reg);
			}
		}
		sun3_address_regs_write_unsigned_long(&vm->address_regs, vm->reg_num, sp);
		vm->pc += sizeof(sun3_unsigned_word);
	}
	break;
	case sun3_instr_mode_ARID:
		sun3_errors_unimplemented_instruction(vm->errors);
		break;
	case sun3_instr_mode_ARII:
		sun3_errors_unimplemented_instruction(vm->errors);
		break;
	case sun3_instr_mode_EXTD:
		switch(vm->reg_num) {
		case sun3_instr_mode_ASA:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		case sun3_instr_mode_ALA:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		case sun3_instr_mode_PCD:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		case sun3_instr_mode_PCI:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		default:
			sun3_errors_illegal_instruction(vm->errors);
			break;
		}
		break;
	default:
		sun3_errors_illegal_instruction(vm->errors);
	}
}


void sun3_movem_mem_to_reg_long(sun3 *vm)
{
	switch(vm->ea_mode) {
	case sun3_instr_mode_ARI:
		sun3_errors_unimplemented_instruction(vm->errors);
		break;
	case sun3_instr_mode_ARIPI:
	{
		sun3_unsigned_word reg_mask= sun3_memory_read_unsigned_word(&vm->memory, vm->pc);
		sun3_memory_address sp= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num);
		unsigned int reg_num;
		for (reg_num= 0; reg_num != 8; reg_num+=1, reg_mask>>=1) {
			if (reg_mask&0x1) {
				sun3_unsigned_long reg= sun3_memory_read_unsigned_long(&vm->memory, sp);
				sun3_data_regs_write_unsigned_long(&vm->data_regs, reg_num, reg);
				sp += sizeof(sun3_unsigned_long);
			}
		}
		for (reg_num= 0; reg_num != 8; reg_num+=1, reg_mask>>=1) {
			if (reg_mask&0x1) {
				sun3_unsigned_long reg= sun3_memory_read_unsigned_long(&vm->memory, sp);
				sun3_address_regs_write_unsigned_long(&vm->address_regs, reg_num, reg);
				sp += sizeof(sun3_unsigned_long);
			}
		}
		sun3_address_regs_write_unsigned_long(&vm->address_regs, vm->reg_num, sp);
		vm->pc += sizeof(sun3_unsigned_word);
	}
	break;
	case sun3_instr_mode_ARIPD:
		sun3_errors_illegal_instruction(vm->errors);
		break;
	case sun3_instr_mode_ARID:
		sun3_errors_unimplemented_instruction(vm->errors);
		break;
	case sun3_instr_mode_ARII:
		sun3_errors_unimplemented_instruction(vm->errors);
		break;
	case sun3_instr_mode_EXTD:
		switch(vm->reg_num) {
		case sun3_instr_mode_ASA:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		case sun3_instr_mode_ALA:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		case sun3_instr_mode_PCD:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		case sun3_instr_mode_PCI:
			sun3_errors_unimplemented_instruction(vm->errors);
			break;
		default:
			sun3_errors_illegal_instruction(vm->errors);
			break;
		}
		break;
	default:
		sun3_errors_illegal_instruction(vm->errors);
	}
}
