#include <assert.h>
#include <stddef.h>
#include <stdio.h>
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
#include "sun3_bcc_or_bsr.h"

void sun3_bcc_or_bsr(sun3 *vm)
{
	int branch;

	switch (sun3_instr_condition(vm->pci)) {
	case sun3_condition_T:
		branch= 1;
		break;
	case sun3_condition_F:
	{
		/*
		** BSR(B) is hidden where BF should be.
		*/
		sun3_memory_address sp= sun3_address_regs_read_unsigned_long(&vm->address_regs, 7);
		sp -= sizeof(sun3_memory_address);
		sun3_address_regs_write_unsigned_long(&vm->address_regs, 7, sp);
		{
			sun3_signed_byte disp= ((sun3_signed_byte)((sun3_unsigned_byte)vm->pci));
			if (disp == 0) {
				sun3_memory_write_long(&vm->memory, sp, vm->pc+sizeof(sun3_unsigned_word));
				vm->pc += sun3_memory_read_signed_word(&vm->memory, vm->pc);
			} else {
				sun3_memory_write_long(&vm->memory, sp, vm->pc);
				vm->pc += disp;
			}
		}
	}
	return;
	case sun3_condition_HI:
		branch= !sun3_sr_c(vm->sr) & !sun3_sr_z(vm->sr);
		break;
	case sun3_condition_LS:
		branch= sun3_sr_c(vm->sr) | sun3_sr_z(vm->sr);
		break;
	case sun3_condition_CC:
		branch= !sun3_sr_c(vm->sr);
		break;
	case sun3_condition_CS:
		branch= sun3_sr_c(vm->sr);
		break;
	case sun3_condition_NE:
		branch= !sun3_sr_z(vm->sr);
		break;
	case sun3_condition_EQ:
		branch= sun3_sr_z(vm->sr);
		break;
	case sun3_condition_VC:
		branch= !sun3_sr_v(vm->sr);
		break;
	case sun3_condition_VS:
		branch= sun3_sr_v(vm->sr);
		break;
	case sun3_condition_PL:
		branch= !sun3_sr_n(vm->sr);
		break;
	case sun3_condition_MI:
		branch= sun3_sr_n(vm->sr);
		break;
	case sun3_condition_GE:
		branch= (sun3_sr_n(vm->sr) & sun3_sr_v(vm->sr))  |  (!sun3_sr_n(vm->sr) & !sun3_sr_v(vm->sr));
		break;
	case sun3_condition_LT:
		branch= (sun3_sr_n(vm->sr) & (!sun3_sr_v(vm->sr)))  |  ((!sun3_sr_n(vm->sr)) & sun3_sr_v(vm->sr));
		break;
	case sun3_condition_GT:
		branch= (sun3_sr_n(vm->sr) & sun3_sr_v(vm->sr) & !sun3_sr_z(vm->sr))  |  (!sun3_sr_n(vm->sr) & !sun3_sr_v(vm->sr) & !sun3_sr_z(vm->sr));
		break;
	case sun3_condition_LE:
		branch= sun3_sr_z(vm->sr) | (sun3_sr_n(vm->sr) & (!sun3_sr_v(vm->sr)))  |  ((!sun3_sr_n(vm->sr)) & sun3_sr_v(vm->sr));
		break;
	default:
		assert(0);
	}
	if (branch) {
		sun3_signed_byte disp= ((sun3_signed_byte)((sun3_unsigned_byte)vm->pci));
		if (disp == 0) {
			vm->pc += sun3_memory_read_signed_word(&vm->memory, vm->pc);
		} else {
			vm->pc += disp;
		}
	} else if ((sun3_unsigned_byte)vm->pci == 0) { 
		vm->pc += sizeof(sun3_unsigned_word);
	}
}
