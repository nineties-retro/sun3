#include <stddef.h>
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_memory_write.h"
#include "sun3_data_regs.h"
#include "sun3_address_regs.h"
#include "sun3_sr.h"
#include "sun3.h"
#include "sun3_instr.h"
#include "sun3_instruction.h"
#include "sun3_swap.h"

void sun3_instr_swap(sun3 *vm)
{
	sun3_signed_long v;

	v = sun3_data_regs_read_unsigned_long(&vm->data_regs, vm->reg_num);
	v = (v<<16)|(v>>16);
	sun3_data_regs_write_unsigned_long(&vm->data_regs, vm->reg_num, v);
	sun3_sr_set_v(vm->sr, 0);
	sun3_sr_set_c(vm->sr, 0);
	sun3_sr_set_z(vm->sr, v==0);
	sun3_sr_set_n(vm->sr, ((sun3_signed_long)v)<0);
}
