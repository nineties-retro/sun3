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
#include "sun3_ea_mode_read_long.h"
#include "sun3_ea_mode_rw_long.h"
#include "sun3_and_long.h"

sun3_long_instruction sun3_instr_and_reg_long =
{ 0
, 0
, 0
, sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd
  |sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala
  |sun3_ea_mode_pcd|sun3_ea_mode_pci|sun3_ea_mode_id
, 0
, 0
, 0
};


void sun3_and_reg_long(sun3 *vm)
{
	unsigned int dest_reg= sun3_instr_move_reg(vm->pci);
	sun3_unsigned_long dest= sun3_data_regs_read_unsigned_long(&vm->data_regs, dest_reg);
	sun3_unsigned_long ans, src;
	sun3_ea_mode_read_long(vm, &sun3_instr_and_reg_long);
	src= sun3_instr_and_reg_long.src;
	ans= dest&src;
	sun3_data_regs_write_unsigned_long(&vm->data_regs, dest_reg, ans);
	sun3_sr_set_n(vm->sr, sun3_unsigned_byte_sign(ans));
	sun3_sr_set_z(vm->sr, ans==0);
	sun3_sr_set_v(vm->sr, 0);
	sun3_sr_set_c(vm->sr, 0);
}


static void sun3_instr_and_ea_long_execute(sun3_long_instruction *or)
{
	or->ans= or->src&or->dest;
}



static void sun3_instr_and_ea_long_set_flags(sun3_long_instruction *or, sun3_sr *sr)
{
	sun3_sr_set_n(*sr, sun3_unsigned_byte_sign(or->ans));
	sun3_sr_set_z(*sr, or->ans==0);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


static sun3_long_instruction sun3_instr_and_ea_long =
{ 0
, 0
, 0
, sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd
  |sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala
, sun3_instr_and_ea_long_execute
, sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd
  |sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala
, sun3_instr_and_ea_long_set_flags
};


void sun3_and_ea_long(sun3 *vm)
{
	unsigned int src_reg= sun3_instr_move_reg(vm->pci);
	sun3_instr_and_ea_long.src= sun3_data_regs_read_unsigned_long(&vm->data_regs, src_reg);
	sun3_ea_mode_rw_long(vm, &sun3_instr_and_ea_long);
}
