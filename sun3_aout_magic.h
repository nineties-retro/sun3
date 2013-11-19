#ifndef SUN3_AOUT_MAGIC_H
#define SUN3_AOUT_MAGIC_H

/*
 * The various sun3_aout_header.magic values that sun3 can support.
 * For some reason they are always given in octal in other header
 * files so the tradition is carried on here ...
 */

/*
 * An object file or impure executable.
 */
#define sun3_aout_OMAGIC 0407

/*
 * A (demand-paged) executable.
 */
#define sun3_aout_ZMAGIC 0413

/*
 * pure executable, read-only text 
 */
#define sun3_aout_NMAGIC 0410

#endif
