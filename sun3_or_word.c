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
#include "sun3_ea_mode_read_word.h"
#include "sun3_ea_mode_rw_word.h"
#include "sun3_or_word.h"

sun3_word_instruction sun3_instr_or_reg_word = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala|sun3_ea_mode_pcd|sun3_ea_mode_pci|sun3_ea_mode_id
};


void sun3_or_reg_word(sun3 *vm)
{
	unsigned int dest_reg = sun3_instr_move_reg(vm->pci);
	sun3_unsigned_word dest = sun3_data_regs_read_unsigned_word(&vm->data_regs, dest_reg);
	sun3_unsigned_word ans, src;

	sun3_ea_mode_read_word(vm, &sun3_instr_or_reg_word);
	src = sun3_instr_or_reg_word.src;
	ans = dest|src;
	sun3_data_regs_write_unsigned_word(&vm->data_regs, dest_reg, ans);
	sun3_sr_set_n(vm->sr, sun3_unsigned_byte_sign(ans));
	sun3_sr_set_z(vm->sr, ans==0);
	sun3_sr_set_v(vm->sr, 0);
	sun3_sr_set_c(vm->sr, 0);
}


static void sun3_instr_or_ea_word_execute(sun3_word_instruction * or)
{
	or->ans= or->src|or->dest;
}



static void sun3_instr_or_ea_word_set_flags(sun3_word_instruction * or, sun3_sr * sr)
{
	sun3_sr_set_n(*sr, sun3_unsigned_byte_sign(or->ans));
	sun3_sr_set_z(*sr, or->ans==0);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


static sun3_word_instruction sun3_instr_or_ea_word = {
	.legal_execute_modes = sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_or_ea_word_execute,
	.legal_flag_modes = sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_or_ea_word_set_flags
};


void sun3_or_ea_word(sun3 *vm)
{
	unsigned int src_reg = sun3_instr_move_reg(vm->pci);
	sun3_instr_or_ea_word.src = sun3_data_regs_read_unsigned_word(&vm->data_regs, src_reg);
	sun3_ea_mode_rw_word(vm, &sun3_instr_or_ea_word);
}
