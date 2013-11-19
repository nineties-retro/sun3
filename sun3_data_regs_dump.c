#include <stddef.h>
#include <stdio.h>
#include "sun3_machine_types.h"
#include "sun3_data_regs.h"
#include "sun3_data_regs_dump.h"

void sun3_data_regs_dump(sun3_data_regs *regs, FILE *sink)
{
	size_t i= 8;
	for (; i != 0; i-=1) {
		sun3_unsigned_long data= sun3_data_regs_read_unsigned_long(regs, i-1);
		fprintf(sink, "D%ld %08"sun3_unsigned_long_fmtx"\n", (unsigned long)(i-1), data);
	}
}
