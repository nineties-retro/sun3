#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_ea_mode_rw_byte.h"
#include "sun3_scc.h"

static void sun3_instr_scc_execute(sun3_byte_instruction * scc)
{
	scc->ans= 0xff;
}


sun3_byte_instruction sun3_instr_scc = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_scc_execute
};

void sun3_scc(sun3 *vm)
{
	int set;
	switch (sun3_instr_condition(vm->pci)) {
	case sun3_condition_T:
		set= 1;
		break;
	case sun3_condition_F:
		set= 0;
		break;
	case sun3_condition_HI:
		set= !sun3_sr_c(vm->sr) & !sun3_sr_z(vm->sr);
		break;
	case sun3_condition_LS:
		set= sun3_sr_c(vm->sr) | sun3_sr_z(vm->sr);
		break;
	case sun3_condition_CC:
		set= !sun3_sr_c(vm->sr);
		break;
	case sun3_condition_CS:
		set= sun3_sr_c(vm->sr);
		break;
	case sun3_condition_NE:
		set= !sun3_sr_z(vm->sr);
		break;
	case sun3_condition_EQ:
		set= sun3_sr_z(vm->sr);
		break;
	case sun3_condition_VC:
		set= !sun3_sr_v(vm->sr);
		break;
	case sun3_condition_VS:
		set= sun3_sr_v(vm->sr);
		break;
	case sun3_condition_PL:
		set= !sun3_sr_n(vm->sr);
		break;
	case sun3_condition_MI:
		set= sun3_sr_n(vm->sr);
		break;
	case sun3_condition_GE:
		set= (sun3_sr_n(vm->sr) & sun3_sr_v(vm->sr))  |  (!sun3_sr_n(vm->sr) & !sun3_sr_v(vm->sr));
		break;
	case sun3_condition_LT:
		set= (sun3_sr_n(vm->sr) & (!sun3_sr_v(vm->sr)))  |  ((!sun3_sr_n(vm->sr)) & sun3_sr_v(vm->sr));
		break;
	case sun3_condition_GT:
		set= (sun3_sr_n(vm->sr) & sun3_sr_v(vm->sr) & (!sun3_sr_z(vm->sr)))  |  (!sun3_sr_n(vm->sr) & (!sun3_sr_v(vm->sr)) & (!sun3_sr_z(vm->sr)));
		break;
	case sun3_condition_LE:
		set= sun3_sr_z(vm->sr) | (sun3_sr_n(vm->sr) & (!sun3_sr_v(vm->sr)))  |  ((!sun3_sr_n(vm->sr)) & sun3_sr_v(vm->sr));
		break;
	default:
		assert(0);
	}
	if (set) {
		sun3_ea_mode_rw_byte(vm, &sun3_instr_scc);
	}
}
