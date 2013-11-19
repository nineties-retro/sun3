#ifndef SUN3_MEMORY_IO_H
#define SUN3_MEMORY_IO_H

/* XH: stddef.h */
/* XH: sun3_memory.h */

typedef int sun3_memory_file;

int sun3_memory_io_open(char const *, sun3_memory_file *);

int sun3_memory_io_read(sun3_memory_file, sun3_memory *, sun3_memory_address, size_t);

int sun3_memory_io_write(sun3_memory_file, sun3_memory *, sun3_memory_address, size_t);

int sun3_memory_io_close(sun3_memory_file);

#endif
