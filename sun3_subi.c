#include "sun3_instruction.h"

static void sun3_instr_subi_byte_execute(sun3_byte_instruction * subi)
{
	subi->ans= subi->dest-subi->src;
}



static void sun3_instr_subi_byte_set_flags(sun3_byte_instruction * subi, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( ( (!sun3_unsigned_byte_sign(subi->src))
			     &  sun3_unsigned_byte_sign(subi->dest)
			     & (!sun3_unsigned_byte_sign(subi->ans))
			     )
			   | (  sun3_unsigned_byte_sign(subi->src)
			     & (!sun3_unsigned_byte_sign(subi->dest))
			     &  sun3_unsigned_byte_sign(subi->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_byte_sign(subi->src)
			     & (!sun3_unsigned_byte_sign(subi->dest))
			     )
			   | (  sun3_unsigned_byte_sign(subi->ans)
			     & (!sun3_unsigned_byte_sign(subi->dest))
			     )
			   | (  sun3_unsigned_byte_sign(subi->src)
			     &  sun3_unsigned_byte_sign(subi->ans)
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, subi->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_byte)subi->ans)<0);
}



sun3_byte_instruction sun3_instr_subi_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_subi_byte_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_subi_byte_set_flags
};



static void sun3_instr_subi_word_execute(sun3_word_instruction * subi)
{
	subi->ans= subi->dest-subi->src;
}



static void sun3_instr_subi_word_set_flags(sun3_word_instruction * subi, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( ( (!sun3_unsigned_word_sign(subi->src))
			     &  sun3_unsigned_word_sign(subi->dest)
			     & (!sun3_unsigned_word_sign(subi->ans))
			     )
			   | (  sun3_unsigned_word_sign(subi->src)
			     & (!sun3_unsigned_word_sign(subi->dest))
			     &  sun3_unsigned_word_sign(subi->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_word_sign(subi->src)
			     & (!sun3_unsigned_word_sign(subi->dest))
			     )
			   | (  sun3_unsigned_word_sign(subi->ans)
			     & (!sun3_unsigned_word_sign(subi->dest))
			     )
			   | (  sun3_unsigned_word_sign(subi->src)
			     &  sun3_unsigned_word_sign(subi->ans)
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, subi->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_word)subi->ans)<0);
}


sun3_word_instruction sun3_instr_subi_word = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_subi_word_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_subi_word_set_flags
};


static void sun3_instr_subi_long_execute(sun3_long_instruction * subi)
{
	subi->ans= subi->dest-subi->src;
}



static void sun3_instr_subi_long_set_flags(sun3_long_instruction * subi, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( ( (!sun3_unsigned_long_sign(subi->src))
			     &  sun3_unsigned_long_sign(subi->dest)
			     & (!sun3_unsigned_long_sign(subi->ans))
			     )
			   | (  sun3_unsigned_long_sign(subi->src)
			     & (!sun3_unsigned_long_sign(subi->dest))
			     &  sun3_unsigned_long_sign(subi->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_long_sign(subi->src)
			     & (!sun3_unsigned_long_sign(subi->dest))
			     )
			   | (  sun3_unsigned_long_sign(subi->ans)
			     & (!sun3_unsigned_long_sign(subi->dest))
			     )
			   | (  sun3_unsigned_long_sign(subi->src)
			     &  sun3_unsigned_long_sign(subi->ans)
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, subi->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_long)subi->ans)<0);
}


sun3_long_instruction sun3_instr_subi_long = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_subi_long_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_subi_long_set_flags
};
