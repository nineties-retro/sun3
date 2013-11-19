#ifndef SUN3_MEMORY_READ_H
#define SUN3_MEMORY_READ_H

sun3_unsigned_long sun3_memory_read_unsigned_long(sun3_memory *, sun3_memory_address);

sun3_signed_long sun3_memory_read_signed_long(sun3_memory *, sun3_memory_address);

sun3_unsigned_word sun3_memory_read_unsigned_word(sun3_memory *, sun3_memory_address);

sun3_signed_word sun3_memory_read_signed_word(sun3_memory *, sun3_memory_address);

sun3_unsigned_byte sun3_memory_read_unsigned_byte(sun3_memory *, sun3_memory_address);

sun3_signed_byte sun3_memory_read_signed_byte(sun3_memory *, sun3_memory_address);

#endif
