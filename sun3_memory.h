#ifndef SUN3_MEMORY_H
#define SUN3_MEMORY_H

/*
 * MC68000 Virtual Machine memory
 */

#include "sun3_machine_types.h"

typedef sun3_unsigned_long sun3_memory_address;

#define sun3_memory_address_fmtx sun3_unsigned_long_fmtx

struct sun3_errors;

struct sun3_memory {
	sun3_unsigned_byte  * data;
	sun3_memory_address   read_start;
	sun3_memory_address   write_start;
	sun3_memory_address   end;
	size_t                nbytes;
	struct sun3_errors   * errors;
};

typedef struct sun3_memory sun3_memory;

#define sun3_memory_page_size     0x2000
#define sun3_memory_segment_size 0x20000

int sun3_memory_inv(struct sun3_memory *);

void sun3_memory_init(struct sun3_memory *, struct sun3_errors *);

void sun3_memory_set_size(struct sun3_memory *, size_t);

int sun3_memory_open(struct sun3_memory *);

void sun3_memory_bss(struct sun3_memory *, sun3_memory_address, size_t);

void sun3_memory_close(struct sun3_memory *);

#endif
