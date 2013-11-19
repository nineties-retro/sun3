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
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_ea_mode_read_word.h"
#include "sun3_ea_mode_rw_word.h"
#include "sun3_add_word.h"

static sun3_word_instruction sun3_instr_add_reg_word = {
	0, 
	0,
	0,
	sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd
	|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala
	|sun3_ea_mode_pcd|sun3_ea_mode_pci|sun3_ea_mode_id,
	0,
	0,
	0
};


void sun3_add_reg_word(sun3 *vm)
{
	unsigned int dest_reg= sun3_instr_move_reg(vm->pci);
	sun3_unsigned_word dest= sun3_data_regs_read_unsigned_word(&vm->data_regs, dest_reg);
	sun3_unsigned_word ans, src;
	sun3_ea_mode_read_word(vm, &sun3_instr_add_reg_word);
	src= sun3_instr_add_reg_word.src;
	ans= dest+src;
	sun3_data_regs_write_unsigned_word(&vm->data_regs, dest_reg, ans);
	sun3_sr_set_v(vm->sr, ( (  sun3_unsigned_word_sign(src)
			       &  sun3_unsigned_word_sign(dest)
			       & (!sun3_unsigned_word_sign(ans))
			       )
			     | ( (!sun3_unsigned_word_sign(src))
			       & (!sun3_unsigned_word_sign(dest))
			       &  sun3_unsigned_word_sign(ans)
			       )
			     ));
	sun3_sr_set_c(vm->sr, ( (  sun3_unsigned_word_sign(src)
			       &  sun3_unsigned_word_sign(dest)
			       )
			     | ( (!sun3_unsigned_word_sign(ans))
			       &  sun3_unsigned_word_sign(dest)
			       )
			     | (  sun3_unsigned_word_sign(src)
			       & (!sun3_unsigned_word_sign(ans))
			       )
			     ));
	sun3_sr_set_x(vm->sr, sun3_sr_c(vm->sr));
	sun3_sr_set_z(vm->sr, ans==0);
	sun3_sr_set_n(vm->sr, ((sun3_signed_word)ans)<0);
}


static void sun3_instr_add_ea_word_execute(sun3_word_instruction * add)
{
	add->ans = add->src+add->dest;
}



static void sun3_instr_add_ea_word_set_flags(sun3_word_instruction * add, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( (  sun3_unsigned_word_sign(add->src)
			    &  sun3_unsigned_word_sign(add->dest)
			    & (!sun3_unsigned_word_sign(add->ans))
			    )
			  | ( (!sun3_unsigned_word_sign(add->src))
			    & (!sun3_unsigned_word_sign(add->dest))
			    &  sun3_unsigned_word_sign(add->ans)
			    )
			  ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_word_sign(add->src)
			    &  sun3_unsigned_word_sign(add->dest)
			    )
			  | ( (!sun3_unsigned_word_sign(add->ans))
			    &  sun3_unsigned_word_sign(add->dest)
			    )
			  | (  sun3_unsigned_word_sign(add->src)
			    & (!sun3_unsigned_word_sign(add->ans))
			    )
			  ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, add->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_word)add->ans)<0);
}


static sun3_word_instruction sun3_instr_add_ea_word = {
	0,
	0,
	0,
	sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd
	|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	sun3_instr_add_ea_word_execute,
	sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd
	|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	sun3_instr_add_ea_word_set_flags
};


void sun3_add_ea_word(sun3 *vm)
{
	unsigned int src_reg= sun3_instr_move_reg(vm->pci);
	sun3_instr_add_ea_word.src= sun3_data_regs_read_unsigned_word(&vm->data_regs, src_reg);
	sun3_ea_mode_rw_word(vm, &sun3_instr_add_ea_word);
}
