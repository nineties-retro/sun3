#include "sun3_instruction.h"

static void sun3_instr_addi_byte_execute(struct sun3_byte_instruction * addi)
{
	addi->ans= addi->dest+addi->src;
}


static void sun3_instr_addi_byte_set_flags(struct sun3_byte_instruction *addi, sun3_sr *sr)
{
	sun3_sr_set_v(*sr, ( (  sun3_unsigned_byte_sign(addi->src)
			     &  sun3_unsigned_byte_sign(addi->dest)
			     & !sun3_unsigned_byte_sign(addi->ans)
			     )
			   | ( !sun3_unsigned_byte_sign(addi->src)
			     & !sun3_unsigned_byte_sign(addi->dest)
			     &  sun3_unsigned_byte_sign(addi->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_byte_sign(addi->src)
			     &  sun3_unsigned_byte_sign(addi->dest)
			     )
			   | ( (!sun3_unsigned_byte_sign(addi->ans))
			     &  sun3_unsigned_byte_sign(addi->dest)
			     )
			   | (  sun3_unsigned_byte_sign(addi->src)
			     & (!sun3_unsigned_byte_sign(addi->ans))
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, addi->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_byte)addi->ans)<0);
}



sun3_byte_instruction sun3_instr_addi_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_addi_byte_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_addi_byte_set_flags
};



static void sun3_instr_addi_word_execute(sun3_word_instruction * addi)
{
	addi->ans= addi->dest+addi->src;
}



static void sun3_instr_addi_word_set_flags(struct sun3_word_instruction *addi, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( (  sun3_unsigned_word_sign(addi->src)
			     &  sun3_unsigned_word_sign(addi->dest)
			     & !sun3_unsigned_word_sign(addi->ans)
			     )
			   | ( !sun3_unsigned_word_sign(addi->src)
			     & !sun3_unsigned_word_sign(addi->dest)
			     &  sun3_unsigned_word_sign(addi->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_word_sign(addi->src)
			     &  sun3_unsigned_word_sign(addi->dest)
			     )
			   | ( (!sun3_unsigned_word_sign(addi->ans))
			     &  sun3_unsigned_word_sign(addi->dest)
			     )
			   | (  sun3_unsigned_word_sign(addi->src)
			     & (!sun3_unsigned_word_sign(addi->ans))
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, addi->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_word)addi->ans)<0);
}


sun3_word_instruction sun3_instr_addi_word = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_addi_word_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_addi_word_set_flags
};


static void sun3_instr_addi_long_execute(sun3_long_instruction * addi)
{
	addi->ans= addi->dest-addi->src;
}



static void sun3_instr_addi_long_set_flags(sun3_long_instruction * addi, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( (  sun3_unsigned_long_sign(addi->src)
			     &  sun3_unsigned_long_sign(addi->dest)
			     & !sun3_unsigned_long_sign(addi->ans)
			     )
			   | ( !sun3_unsigned_long_sign(addi->src)
			     & !sun3_unsigned_long_sign(addi->dest)
			     &  sun3_unsigned_long_sign(addi->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_long_sign(addi->src)
			     &  sun3_unsigned_long_sign(addi->dest)
			     )
			   | ( (!sun3_unsigned_long_sign(addi->ans))
			     &  sun3_unsigned_long_sign(addi->dest)
			     )
			   | (  sun3_unsigned_long_sign(addi->src)
			     & (!sun3_unsigned_long_sign(addi->ans))
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, addi->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_long)addi->ans)<0);
}


sun3_long_instruction sun3_instr_addi_long = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_addi_long_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_addi_long_set_flags
};
