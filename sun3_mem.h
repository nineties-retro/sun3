#ifndef SUN3_MEM_H
#define SUN3_MEM_H

#include <stdlib.h>

#define sun3_mem_malloc(n_bytes) malloc(n_bytes)

#define sun3_mem_realloc(addr, n_bytes) realloc(addr, n_bytes)

#define sun3_mem_free(addr)  free(addr)

#endif
