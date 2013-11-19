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
#include "sun3_ea_mode_read_long.h"
#include "sun3_suba.h"

sun3_word_instruction sun3_instr_suba_word = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ard|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala|sun3_ea_mode_pcd|sun3_ea_mode_pci|sun3_ea_mode_id
};


/*
 * Note that although the following implements suba.w, it does the arithmetic
 * in 32-bits.  See Krantz&Stanley pp 56.
 */
void sun3_suba_word(sun3 *vm)
{
	unsigned int dest_reg= sun3_instr_move_reg(vm->pci);
	sun3_signed_long dest= sun3_address_regs_read_signed_word(&vm->address_regs, dest_reg);
	sun3_signed_long ans, src;
	sun3_ea_mode_read_word(vm, &sun3_instr_suba_word);
	src= (sun3_signed_long)((sun3_signed_word)sun3_instr_suba_word.src);
	ans= dest-src;
	sun3_address_regs_write_signed_word(&vm->address_regs, dest_reg, ans);
	sun3_sr_set_v(vm->sr, ( ( (!sun3_unsigned_long_sign(src))
				&  sun3_unsigned_long_sign(dest)
				& (!sun3_unsigned_long_sign(ans))
				)
			      | (  sun3_unsigned_long_sign(src)
			        & (!sun3_unsigned_long_sign(dest))
				&  sun3_unsigned_long_sign(ans)
				)
			      ));
	sun3_sr_set_c(vm->sr, ( (  sun3_unsigned_long_sign(src)
				& (!sun3_unsigned_long_sign(dest))
				)
			      | (  sun3_unsigned_long_sign(ans)
				& (!sun3_unsigned_long_sign(dest))
				)
			      | (  sun3_unsigned_long_sign(src)
				&  sun3_unsigned_long_sign(ans)
				)
			      ));
	sun3_sr_set_x(vm->sr, sun3_sr_c(vm->sr));
	sun3_sr_set_z(vm->sr, ans==0);
	sun3_sr_set_n(vm->sr, ((sun3_signed_long)ans)<0);
}


sun3_long_instruction sun3_instr_suba_long = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ard|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala|sun3_ea_mode_pcd|sun3_ea_mode_pci|sun3_ea_mode_id
};


void sun3_suba_long(sun3 *vm)
{
	unsigned int dest_reg= sun3_instr_move_reg(vm->pci);
	sun3_signed_long dest= sun3_address_regs_read_signed_long(&vm->address_regs, dest_reg);
	sun3_signed_long ans, src;
	sun3_ea_mode_read_long(vm, &sun3_instr_suba_long);
	src= sun3_instr_suba_long.src;
	ans= dest-src;
	sun3_address_regs_write_signed_word(&vm->address_regs, dest_reg, ans);
	sun3_sr_set_v(vm->sr, ( ( (!sun3_unsigned_long_sign(src))
				&  sun3_unsigned_long_sign(dest)
				& (!sun3_unsigned_long_sign(ans))
				)
			      | (  sun3_unsigned_long_sign(src)
				& (!sun3_unsigned_long_sign(dest))
				&  sun3_unsigned_long_sign(ans)
				)
			      ));
	sun3_sr_set_c(vm->sr, ( (  sun3_unsigned_long_sign(src)
				& (!sun3_unsigned_long_sign(dest))
				)
			      | (  sun3_unsigned_long_sign(ans)
				& (!sun3_unsigned_long_sign(dest))
			        )
			      | (  sun3_unsigned_long_sign(src)
				&  sun3_unsigned_long_sign(ans)
				)
			      ));
	sun3_sr_set_x(vm->sr, sun3_sr_c(vm->sr));
	sun3_sr_set_z(vm->sr, ans==0);
	sun3_sr_set_n(vm->sr, ((sun3_signed_long)ans)<0);
}
