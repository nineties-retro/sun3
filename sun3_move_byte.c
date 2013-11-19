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
#include "sun3_ea_mode_read_byte.h"
#include "sun3_move_byte.h"
#include "sun3_trace.h"

static sun3_byte_instruction sun3_instr_move_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ard|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa| sun3_ea_mode_ala|sun3_ea_mode_pcd|sun3_ea_mode_pci|sun3_ea_mode_id
};


void sun3_move_byte(sun3 *vm) 
{
	sun3_ea_mode_read_byte(vm, &sun3_instr_move_byte);

	switch (sun3_instr_move_mode(vm->pci)) {
	case sun3_instr_mode_DRD:
	{
		unsigned int reg = sun3_instr_move_reg(vm->pci);
		sun3_trace_instr(vm, "move.b %"sun3_unsigned_byte_fmtx", d%u", sun3_instr_move_byte.src, reg);
		sun3_data_regs_write_signed_byte(&vm->data_regs, reg, sun3_instr_move_byte.src);
		break;
	}
	case sun3_instr_mode_ARD:
	{
		unsigned int reg = sun3_instr_move_reg(vm->pci);
		sun3_trace_instr(vm, "move.b %"sun3_unsigned_byte_fmtx", a%u", sun3_instr_move_byte.src, reg);
		sun3_address_regs_write_signed_byte(&vm->address_regs, reg, sun3_instr_move_byte.src);
		return;			/* MOVEA doesn't set flags */
	}
	case sun3_instr_mode_ARI:
	{
		unsigned int reg = sun3_instr_move_reg(vm->pci);
		sun3_memory_address addr = sun3_address_regs_read_unsigned_long(&vm->address_regs, reg);
		sun3_trace_instr(vm, "move.b %"sun3_unsigned_byte_fmtx", a%u", sun3_instr_move_byte.src, reg);
		sun3_memory_write_byte(&vm->memory, addr, sun3_instr_move_byte.src);
		break;
	}
	case sun3_instr_mode_ARIPI:
	{
		unsigned int reg = sun3_instr_move_reg(vm->pci);
		sun3_memory_address addr = sun3_address_regs_read_unsigned_long(&vm->address_regs, reg);
		sun3_address_regs_write_unsigned_long(&vm->address_regs, reg, addr+sizeof(sun3_unsigned_byte));
		sun3_trace_instr(vm, "move.b %"sun3_unsigned_byte_fmtx", (a%u)+", sun3_instr_move_byte.src, reg);
		sun3_memory_write_byte(&vm->memory, addr, sun3_instr_move_byte.src);
		break;
	}
	case sun3_instr_mode_ARIPD:
	{
		unsigned int reg = sun3_instr_move_reg(vm->pci);
		sun3_memory_address addr = sun3_address_regs_read_unsigned_long(&vm->address_regs, reg);
		addr -= sizeof(sun3_unsigned_byte);
		sun3_address_regs_write_unsigned_long(&vm->address_regs, reg, addr);
		addr = sun3_address_regs_read_unsigned_long(&vm->address_regs, reg);
		sun3_trace_instr(vm, "move.b %"sun3_unsigned_byte_fmtx", -(a%u)", sun3_instr_move_byte.src, reg);
		sun3_memory_write_byte(&vm->memory, addr, sun3_instr_move_byte.src);
		break;
	}
	case sun3_instr_mode_ARID:
	{
		unsigned int reg = sun3_instr_move_reg(vm->pci);
		sun3_memory_address addr = sun3_address_regs_read_unsigned_long(&vm->address_regs, reg);
		addr += (sun3_signed_long)sun3_memory_read_signed_word(&vm->memory, vm->pc);
		sun3_memory_write_byte(&vm->memory, addr, sun3_instr_move_byte.src);
		vm->pc += sizeof(sun3_signed_word);
		break;
	}
	case sun3_instr_mode_ARII:
	{
		sun3_unsigned_word npci= sun3_memory_read_signed_word(&vm->memory, vm->pc);
		sun3_memory_address addr= sun3_address_regs_read_unsigned_long(&vm->address_regs, vm->reg_num) + (sun3_signed_byte)((sun3_unsigned_byte)npci);
		unsigned int reg= npci>>12;
		sun3_unsigned_long disp = (reg&0x8)
			? sun3_address_regs_read_unsigned_long(&vm->address_regs, reg&0x7)
			: sun3_data_regs_read_unsigned_long(&vm->data_regs, reg);
		addr += (npci&(1<<11))
			? (sun3_signed_long)disp
			: (sun3_signed_long)((sun3_unsigned_word)disp);
		sun3_memory_write_byte(&vm->memory, addr, sun3_instr_move_byte.src);
		vm->pc += sizeof(sun3_signed_word);
		break;
	}
	case sun3_instr_mode_EXTD:
		switch(sun3_instr_move_reg(vm->pci)) {
		case sun3_instr_mode_ASA:
		{
			sun3_memory_address addr = (sun3_memory_address)((sun3_signed_long)sun3_memory_read_signed_word(&vm->memory, vm->pc));
			sun3_memory_write_byte(&vm->memory, addr, sun3_instr_move_byte.src);
			vm->pc += sizeof(sun3_unsigned_word);
		}
		break;
		case sun3_instr_mode_ALA:
		{
			sun3_memory_address addr = (sun3_memory_address)sun3_memory_read_unsigned_long(&vm->memory, vm->pc);
			sun3_memory_write_byte(&vm->memory, addr, sun3_instr_move_byte.src);
			vm->pc += sizeof(sun3_unsigned_long);
		}
		break;
		default:
			sun3_errors_illegal_instruction(vm->errors);
		}
		break;
	default:
		sun3_errors_unimplemented_instruction(vm->errors);
		break;
	}
	sun3_sr_set_n(vm->sr, ((sun3_signed_byte)sun3_instr_move_byte.src)<0);
	sun3_sr_set_z(vm->sr, sun3_instr_move_byte.src==0);
	sun3_sr_set_v(vm->sr, 0);
	sun3_sr_set_c(vm->sr, 0);
}
