#ifndef SUN3_DATA_REGS_H
#define SUN3_DATA_REGS_H

/* XH: stddef.h */
/* XH: sun3_machine_types.h */

#ifndef sun3_data_regs_nregs
#define sun3_data_regs_nregs 8
#endif

struct sun3_data_regs {
  sun3_unsigned_long regs[sun3_data_regs_nregs];
};

typedef struct sun3_data_regs sun3_data_regs;

#ifndef sun3_data_regs_extern
#define sun3_data_regs_extern extern
#endif

sun3_data_regs_extern void
sun3_data_regs_write_unsigned_byte(sun3_data_regs *, size_t, sun3_unsigned_byte);

sun3_data_regs_extern void
sun3_data_regs_write_unsigned_word(sun3_data_regs *, size_t, sun3_unsigned_word);

sun3_data_regs_extern void
sun3_data_regs_write_unsigned_long(sun3_data_regs *, size_t, sun3_unsigned_long);


sun3_data_regs_extern void
sun3_data_regs_write_signed_byte(sun3_data_regs *, size_t, sun3_signed_byte);

sun3_data_regs_extern void
sun3_data_regs_write_signed_word(sun3_data_regs *, size_t, sun3_signed_word);

sun3_data_regs_extern void
sun3_data_regs_write_signed_long(sun3_data_regs *, size_t, sun3_signed_long);


sun3_data_regs_extern sun3_unsigned_byte
sun3_data_regs_read_unsigned_byte(sun3_data_regs *, size_t);

sun3_data_regs_extern sun3_unsigned_word
sun3_data_regs_read_unsigned_word(sun3_data_regs *, size_t);

sun3_data_regs_extern sun3_unsigned_long
sun3_data_regs_read_unsigned_long(sun3_data_regs *, size_t);


sun3_data_regs_extern sun3_signed_byte
sun3_data_regs_read_signed_byte(sun3_data_regs *, size_t);

sun3_data_regs_extern sun3_signed_word
sun3_data_regs_read_signed_word(sun3_data_regs *, size_t);

sun3_data_regs_extern sun3_signed_long
sun3_data_regs_read_signed_long(sun3_data_regs *, size_t);

#endif
