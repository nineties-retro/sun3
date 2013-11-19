#ifndef SUN3_MEMORY_ALIGNMENT_H
#define SUN3_MEMORY_ALIGNMENT_H

/*
 * Functions that determine whether an address aligned to a particular
 * boundary.
 */

#define sun3_memory_address_long_aligned(addr) (((addr)&0x1) == 0)

#define sun3_memory_address_word_aligned(addr) (((addr)&0x1) == 0)

#endif
