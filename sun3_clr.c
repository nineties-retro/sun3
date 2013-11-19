#include "sun3_instruction.h"
#include "sun3_unused_param.h"

static void sun3_instr_clr_byte_execute(sun3_byte_instruction * clr)
{
	clr->ans= 0;
}



static void sun3_instr_clr_byte_set_flags(sun3_byte_instruction *clr sun3_unused_param,
					  sun3_sr * sr)
{
	sun3_sr_set_n(*sr, 0);
	sun3_sr_set_z(*sr, 1);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_byte_instruction sun3_instr_clr_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_clr_byte_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_clr_byte_set_flags
};



static void sun3_instr_clr_word_execute(sun3_word_instruction * clr)
{
	clr->ans= 0;
}



static void sun3_instr_clr_word_set_flags(sun3_word_instruction *clr sun3_unused_param,
					  sun3_sr * sr)
{
	sun3_sr_set_n(*sr, 0);
	sun3_sr_set_z(*sr, 1);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_word_instruction sun3_instr_clr_word = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_clr_word_execute,
	.legal_flag_modes =sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_clr_word_set_flags
};



static void sun3_instr_clr_long_execute(sun3_long_instruction * clr)
{
	clr->ans= 0;
}



static void sun3_instr_clr_long_set_flags(sun3_long_instruction *clr sun3_unused_param,
					  sun3_sr *sr)
{
	sun3_sr_set_n(*sr, 0);
	sun3_sr_set_z(*sr, 1);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_long_instruction sun3_instr_clr_long = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_clr_long_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_clr_long_set_flags
};
