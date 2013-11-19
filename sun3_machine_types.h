#ifndef SUN3_MACHINE_TYPES_H
#define SUN3_MACHINE_TYPES_H

#include <stdint.h>

typedef uint32_t sun3_unsigned_long;
typedef int32_t sun3_signed_long;
#define sun3_signed_long_is_negative(l) ((l)<0)
#define sun3_unsigned_long_sign(ul) ((ul)>>31)
#define sun3_unsigned_long_nbits 32
#define sun3_unsigned_long_fmt "d"
#define sun3_unsigned_long_fmtx "x"

typedef uint16_t sun3_unsigned_word;
typedef int16_t sun3_signed_word;
#define sun3_signed_word_is_negative(l) ((l)<0)
#define sun3_unsigned_word_sign(ul) ((ul)>>15)
#define sun3_unsigned_word_nbits 16
#define sun3_unsigned_word_fmtx "x"

typedef uint8_t  sun3_unsigned_byte;
typedef int8_t  sun3_signed_byte;
typedef unsigned int   sun3_bitfield;
#define sun3_signed_byte_is_negative(byte) ((byte)<0)
#define sun3_unsigned_byte_sign(ul) ((ul)>>7)
#define sun3_unsigned_byte_nbits 8
#define sun3_unsigned_byte_fmtx "x"

#endif
