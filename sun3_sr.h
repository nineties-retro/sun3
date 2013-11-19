#ifndef SUN3_SR_H
#define SUN3_SR_H

#include "sun3_machine_types.h"

typedef sun3_unsigned_word sun3_sr;

#define sun3_sr_c(sr) (((sr)>>0)&0x1)
#define sun3_sr_v(sr) (((sr)>>1)&0x1)
#define sun3_sr_z(sr) (((sr)>>2)&0x1)
#define sun3_sr_n(sr) (((sr)>>3)&0x1)
#define sun3_sr_x(sr) (((sr)>>4)&0x1)

#define sun3_sr_set_c(sr, b) (sr)= ((sr)&~(1<<0))|((b)<<0)
#define sun3_sr_set_v(sr, b) (sr)= ((sr)&~(1<<1))|((b)<<1)
#define sun3_sr_set_z(sr, b) (sr)= ((sr)&~(1<<2))|((b)<<2)
#define sun3_sr_set_n(sr, b) (sr)= ((sr)&~(1<<3))|((b)<<3)
#define sun3_sr_set_x(sr, b) (sr)= ((sr)&~(1<<4))|((b)<<4)

#endif
