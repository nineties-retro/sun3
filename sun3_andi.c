#include "sun3_instruction.h"

static void sun3_instr_andi_byte_execute(sun3_byte_instruction *andi)
{
	andi->ans= andi->src&andi->dest;
}


static void sun3_instr_andi_byte_set_flags(sun3_byte_instruction * andi, sun3_sr * sr)
{
	sun3_sr_set_n(*sr, andi->ans&(1<<(sun3_unsigned_byte_nbits-1)));
	sun3_sr_set_z(*sr, andi->ans==0);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_byte_instruction sun3_instr_andi_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala|sun3_ea_mode_sr,
	.execute = sun3_instr_andi_byte_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_andi_byte_set_flags
};



static void sun3_instr_andi_word_execute(sun3_word_instruction *andi)
{
	andi->ans= andi->src&andi->dest;
}



static void sun3_instr_andi_word_set_flags(sun3_word_instruction *andi, sun3_sr *sr)
{
	sun3_sr_set_n(*sr, andi->ans&(1<<(sun3_unsigned_word_nbits-1)));
	sun3_sr_set_z(*sr, andi->ans==0);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_word_instruction sun3_instr_andi_word = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_andi_word_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_andi_word_set_flags,
};


static void sun3_instr_andi_long_execute(sun3_long_instruction *andi)
{
	andi->ans= andi->src&andi->dest;
}



static void sun3_instr_andi_long_set_flags(sun3_long_instruction *andi, sun3_sr *sr)
{
	sun3_sr_set_n(*sr, andi->ans&(1<<(sun3_unsigned_long_nbits-1)));
	sun3_sr_set_z(*sr, andi->ans==0);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_long_instruction sun3_instr_andi_long = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_andi_long_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_andi_long_set_flags
};
