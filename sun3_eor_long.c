#include "sun3_instruction.h"
#include "sun3_eor_long.h"

static void sun3_instr_eor_long_execute(sun3_long_instruction * eor)
{
	eor->ans= eor->src^eor->dest;
}


static void sun3_instr_eor_long_set_flags(sun3_long_instruction * eor, sun3_sr * sr)
{
	sun3_sr_set_n(*sr, sun3_unsigned_byte_sign(eor->ans));
	sun3_sr_set_z(*sr, eor->ans==0);
	sun3_sr_set_v(*sr, 0);
	sun3_sr_set_c(*sr, 0);
}


sun3_long_instruction sun3_instr_eor_long = {
	.legal_execute_modes = sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_eor_long_execute,
	.legal_flag_modes = sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_eor_long_set_flags
};
