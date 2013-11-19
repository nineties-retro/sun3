#include <stddef.h>
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_ea_mode_read_word.h"
#include "sun3_mul.h"

sun3_word_instruction sun3_instr_mul = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala|sun3_ea_mode_pcd|sun3_ea_mode_pci|sun3_ea_mode_id
};


void sun3_instr_muls(sun3 *vm)
{
	unsigned int dest_reg= sun3_instr_move_reg(vm->pci);
	sun3_signed_word dest= sun3_data_regs_read_signed_word(&vm->data_regs, dest_reg);
	sun3_signed_word src;
	sun3_signed_long ans;
	sun3_ea_mode_read_word(vm, &sun3_instr_mul);
	src= sun3_instr_mul.src;
	ans= src*dest;
	sun3_data_regs_write_signed_long(&vm->data_regs, dest_reg, ans);
	sun3_sr_set_n(vm->sr, ans<0);
	sun3_sr_set_z(vm->sr, ans==0);
	sun3_sr_set_v(vm->sr, 0);
	sun3_sr_set_c(vm->sr, 0);
}


void sun3_instr_mulu(sun3 *vm)
{
	unsigned int dest_reg= sun3_instr_move_reg(vm->pci);
	sun3_unsigned_word dest= sun3_data_regs_read_unsigned_word(&vm->data_regs, dest_reg);
	sun3_unsigned_word src;
	sun3_unsigned_long ans;
	sun3_ea_mode_read_word(vm, &sun3_instr_mul);
	src= sun3_instr_mul.src;
	ans= src*dest;
	sun3_data_regs_write_unsigned_long(&vm->data_regs, dest_reg, ans);
	sun3_sr_set_n(vm->sr, sun3_unsigned_long_sign(ans));
	sun3_sr_set_z(vm->sr, ans==0);
	sun3_sr_set_v(vm->sr, 0);
	sun3_sr_set_c(vm->sr, 0);
}
