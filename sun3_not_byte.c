#include "sun3_instruction.h"
#include "sun3_not_byte.h"

static void sun3_instr_not_byte_execute(sun3_byte_instruction * not)
{
	not->ans= ~not->dest;
}



static void sun3_instr_not_byte_set_flags(sun3_byte_instruction * not, sun3_sr * sr)
{
	sun3_sr_set_n(*sr, sun3_unsigned_byte_sign(not->ans));
	sun3_sr_set_z(*sr, not->ans==0);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_byte_instruction sun3_instr_not_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_not_byte_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_not_byte_set_flags
};
