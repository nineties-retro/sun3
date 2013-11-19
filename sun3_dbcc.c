#include <assert.h>
#include <stddef.h>
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_dbcc.h"
#include "sun3_trace.h"

void sun3_instr_dbcc(sun3 * vm)
{
	int branch;
	switch (sun3_instr_condition(vm->pci)) {
	case sun3_condition_T:
		branch = 1;
		break;
	case sun3_condition_F:
		branch = 0;
		break;
	case sun3_condition_HI:
		branch = !sun3_sr_c(vm->sr) & !sun3_sr_z(vm->sr);
		break;
	case sun3_condition_LS:
		branch = sun3_sr_c(vm->sr) | sun3_sr_z(vm->sr);
		break;
	case sun3_condition_CC:
		branch = !sun3_sr_c(vm->sr);
		break;
	case sun3_condition_CS:
		branch = sun3_sr_c(vm->sr);
		break;
	case sun3_condition_NE:
		branch = !sun3_sr_z(vm->sr);
		break;
	case sun3_condition_EQ:
		branch = sun3_sr_z(vm->sr);
		break;
	case sun3_condition_VC:
		branch = !sun3_sr_v(vm->sr);
		break;
	case sun3_condition_VS:
		branch = sun3_sr_v(vm->sr);
		break;
	case sun3_condition_PL:
		branch = !sun3_sr_n(vm->sr);
		break;
	case sun3_condition_MI:
		branch = sun3_sr_n(vm->sr);
		break;
	case sun3_condition_GE:
		branch = (sun3_sr_n(vm->sr) & sun3_sr_v(vm->sr))  |  ((!sun3_sr_n(vm->sr)) & (!sun3_sr_v(vm->sr)));
		break;
	case sun3_condition_LT:
		branch = (sun3_sr_n(vm->sr) & (!sun3_sr_v(vm->sr)))  |  ((!sun3_sr_n(vm->sr)) & sun3_sr_v(vm->sr));
		break;
	case sun3_condition_GT:
		branch = (sun3_sr_n(vm->sr) & sun3_sr_v(vm->sr) & !sun3_sr_z(vm->sr))  |  (!sun3_sr_n(vm->sr) & !sun3_sr_v(vm->sr) & !sun3_sr_z(vm->sr));
		break;
	case sun3_condition_LE:
		branch = sun3_sr_z(vm->sr) | (sun3_sr_n(vm->sr) & (!sun3_sr_v(vm->sr)))  |  ((!sun3_sr_n(vm->sr)) & sun3_sr_v(vm->sr));
		break;
	default:
		assert(0);
	}
	if (branch) {
		vm->pc += sizeof(sun3_unsigned_word);
		sun3_trace_instr(vm, "dbcc %"sun3_memory_address_fmtx"", vm->pc);
	} else {
		sun3_unsigned_word count= sun3_data_regs_read_unsigned_word(&vm->data_regs, vm->reg_num);
		if (count == 0) {
			count -= 1;
			sun3_data_regs_write_unsigned_word(&vm->data_regs, vm->reg_num, count);
			vm->pc += sizeof(sun3_unsigned_word);
			sun3_trace_instr(vm, "dbcc %"sun3_memory_address_fmtx"", vm->pc);
		} else {
			count -= 1;
			sun3_data_regs_write_unsigned_word(&vm->data_regs, vm->reg_num, count);
			vm->pc += sun3_memory_read_signed_word(&vm->memory, vm->pc);
			sun3_trace_instr(vm, "dbcc %"sun3_memory_address_fmtx"", vm->pc);
		}
	}
}
