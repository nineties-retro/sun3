#ifndef SUN3_TRACE_H
#define SUN3_TRACE_H

#include <stdio.h>

#define sun3_trace_pc(vm, pc, pci) \
	do { \
		fprintf(stderr, "\npc %08"sun3_memory_address_fmtx" pci %04"sun3_unsigned_word_fmtx"\t", pc, pci); \
	} while (0)

#define sun3_trace_instr(vm, fmt, args...)	\
	do {					\
		fprintf(stderr, fmt, ##args);	\
	} while (0)

#endif
