#include <stddef.h>
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_memory_write.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_ea_mode_rw_byte.h"
#include "sun3_addq_or_subq_byte.h"

static void sun3_instr_subq_byte_execute(sun3_byte_instruction * subq)
{
	subq->ans= subq->dest-subq->src;
}


static void sun3_instr_subq_byte_set_flags(sun3_byte_instruction * subq, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( ( (!sun3_unsigned_byte_sign(subq->src))
			     &  sun3_unsigned_byte_sign(subq->dest)
			     & (!sun3_unsigned_byte_sign(subq->ans))
			     )
			   | (  sun3_unsigned_byte_sign(subq->src)
			     & (!sun3_unsigned_byte_sign(subq->dest))
			     &  sun3_unsigned_byte_sign(subq->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_byte_sign(subq->src)
			     & (!sun3_unsigned_byte_sign(subq->dest))
			     )
			   | (  sun3_unsigned_byte_sign(subq->ans)
			     & (!sun3_unsigned_byte_sign(subq->dest))
			     )
			   | (  sun3_unsigned_byte_sign(subq->src)
			     &  sun3_unsigned_byte_sign(subq->ans)
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, subq->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_byte)subq->ans)<0);
}


sun3_byte_instruction sun3_instr_subq_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ard|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.execute = sun3_instr_subq_byte_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ard|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_subq_byte_set_flags
};



static void sun3_instr_addq_byte_execute(sun3_byte_instruction * addq)
{
	addq->ans= addq->dest+addq->src;
}


static void
sun3_instr_addq_byte_set_flags(sun3_byte_instruction * addq, sun3_sr * sr)
{
	sun3_sr_set_v(*sr, ( (  sun3_unsigned_byte_sign(addq->src)
			     &  sun3_unsigned_byte_sign(addq->dest)
			     & (!sun3_unsigned_byte_sign(addq->ans))
			     )
			   | ( (!sun3_unsigned_byte_sign(addq->src))
			     & (!sun3_unsigned_byte_sign(addq->dest))
			     &  sun3_unsigned_byte_sign(addq->ans)
			     )
			   ));
	sun3_sr_set_c(*sr, ( (  sun3_unsigned_byte_sign(addq->src)
			     &  sun3_unsigned_byte_sign(addq->dest)
			     )
			   | ( (!sun3_unsigned_byte_sign(addq->ans))
			     &  sun3_unsigned_byte_sign(addq->dest)
			     )
			   | (  sun3_unsigned_byte_sign(addq->src)
			     & !sun3_unsigned_byte_sign(addq->ans)
			     )
			   ));
	sun3_sr_set_x(*sr, sun3_sr_c(*sr));
	sun3_sr_set_z(*sr, addq->ans==0);
	sun3_sr_set_n(*sr, ((sun3_signed_byte)addq->ans)<0);
}


sun3_byte_instruction sun3_instr_addq_byte = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ard|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa| sun3_ea_mode_ala,
	.execute = sun3_instr_addq_byte_execute,
	.legal_flag_modes = sun3_ea_mode_drd|sun3_ea_mode_ard|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala,
	.set_flags = sun3_instr_addq_byte_set_flags
};


void sun3_instr_addq_or_subq_byte(sun3 *vm)
{
	sun3_unsigned_byte src= sun3_instr_qdata(vm->pci);
	if (vm->pci&(1<<8)) {
		sun3_instr_subq_byte.src= src;
		sun3_ea_mode_rw_byte(vm, &sun3_instr_subq_byte);
	} else {
		sun3_instr_addq_byte.src= src;
		sun3_ea_mode_rw_byte(vm, &sun3_instr_addq_byte);
	}
}
