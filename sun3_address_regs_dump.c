#include <stddef.h>
#include <stdio.h>
#include "sun3_machine_types.h"
#include "sun3_address_regs.h"
#include "sun3_address_regs_dump.h"


void sun3_address_regs_dump(sun3_address_regs *regs, FILE *sink)
{
	size_t i= 8;
	for (; i != 0; i-=1) {
		sun3_unsigned_long address= sun3_address_regs_read_unsigned_long(regs, i-1);
		fprintf(sink, "A%ld %08"sun3_unsigned_long_fmtx"\n", (unsigned long)(i-1), address);
	}
}
