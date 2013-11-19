#ifndef SUN3_ADDRESS_REGS_H
#define SUN3_ADDRESS_REGS_H

/* XH: stddef.h */
/* XH: sun3_machine_types.h */

#ifndef sun3_address_regs_nregs
#define sun3_address_regs_nregs 8
#endif

struct sun3_address_regs {
	sun3_unsigned_long regs[sun3_address_regs_nregs];
};

typedef struct sun3_address_regs sun3_address_regs;

void sun3_address_regs_write_signed_byte(sun3_address_regs *, size_t, sun3_signed_byte);

void sun3_address_regs_write_signed_word(sun3_address_regs *, size_t, sun3_signed_word);

void sun3_address_regs_write_signed_long(sun3_address_regs *, size_t, sun3_signed_long);

void sun3_address_regs_write_unsigned_byte(sun3_address_regs *, size_t, sun3_unsigned_byte);

void sun3_address_regs_write_unsigned_word(sun3_address_regs *, size_t, sun3_unsigned_word);

void sun3_address_regs_write_unsigned_long(sun3_address_regs *, size_t, sun3_unsigned_long);

sun3_unsigned_byte sun3_address_regs_read_unsigned_byte(sun3_address_regs *, size_t);

sun3_unsigned_word sun3_address_regs_read_unsigned_word(sun3_address_regs *, size_t);

sun3_unsigned_long sun3_address_regs_read_unsigned_long(sun3_address_regs *, size_t);

sun3_signed_byte sun3_address_regs_read_signed_byte(sun3_address_regs *, size_t);

sun3_signed_word sun3_address_regs_read_signed_word(sun3_address_regs *, size_t);

sun3_signed_long sun3_address_regs_read_signed_long(sun3_address_regs *, size_t);

#endif
