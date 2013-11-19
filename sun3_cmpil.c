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
#include "sun3_ea_mode_read_long.h"
#include "sun3_cmpi.h"

static sun3_long_instruction sun3_instr_cmpi_long = {
	.legal_execute_modes = sun3_ea_mode_drd|sun3_ea_mode_ari|sun3_ea_mode_aripi|sun3_ea_mode_aripd|sun3_ea_mode_arid|sun3_ea_mode_arii|sun3_ea_mode_asa|sun3_ea_mode_ala
};


void sun3_cmpi_long(sun3 *vm)
{
	sun3_signed_long src, dest, ans;

	src = (sun3_signed_long)sun3_memory_read_signed_long(&vm->memory, vm->pc);
	vm->pc += sizeof(sun3_unsigned_long);
	sun3_ea_mode_read_long(vm, &sun3_instr_cmpi_long);
	dest= sun3_instr_cmpi_long.src;
	ans = dest-src;
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
