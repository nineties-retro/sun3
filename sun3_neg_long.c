#include "sun3_instruction.h"
#include "sun3_neg_long.h"


static void sun3_instr_neg_long_execute(sun3_long_instruction * neg)
{
	neg->ans = -neg->dest;
}



static void sun3_instr_neg_long_set_flags(sun3_long_instruction * neg, sun3_sr * sr)
{
	sun3_sr_set_n(*sr, sun3_unsigned_long_sign(neg->ans));
	sun3_sr_set_z(*sr, neg->ans==0);
	sun3_sr_set_v(*sr, sun3_unsigned_long_sign(neg->dest)&sun3_unsigned_long_sign(neg->ans));
	sun3_sr_set_c(*sr, sun3_unsigned_long_sign(neg->dest)|sun3_unsigned_long_sign(neg->ans));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
}


sun3_long_instruction sun3_instr_neg_long = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_neg_long_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_neg_long_set_flags
};
