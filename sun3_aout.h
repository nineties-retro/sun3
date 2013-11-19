#ifndef SUN3_AOUT_H
#define SUN3_AOUT_H

/*
 * A Sun3 compatible a.out description.
 * Derived from the Linux/386 a.out format and the various a.out header
 * files contained in the GNU binutils.
 */

/* XH: sun3_machine_types.h */

/* 
 * A description of the a.out header that appears at the front of
 * every a.out file.
 */

/*
 * Note the use of bitfields in the following only increases the
 * implementation dependence.  However, the following is likely to
 * work correctly on its intended target audience: 32-bit 2s
 * complement machines.
 */

struct sun3_aout_header {
	/*
	 * The flags field could be broken down into its constituent fields
	 * but it is unclear from the available documentation what each bit
	 * means so it has been left as a unit.
	 */
	sun3_unsigned_byte flags;

	/*
	 * The machine/architecture that the a.out is meant to run on.
	 * See later for a definition of the possible values.
	 */
	sun3_unsigned_byte machine_type;

	/*
	 * The magic field indicates the type of a.out file
	 * i.e. whether it an object file, demand-paged executable,
	 * pure executable ... etc.  See sun3_aout_magic.h for the
	 * valid values
	 */
	sun3_unsigned_word magic;

	/* The length of the .text section in bytes. */
	sun3_unsigned_long text_length;

	/* The length of the .data section in bytes. */
	sun3_unsigned_long data_length;

	/* The length of the .bss section in bytes. */
	sun3_unsigned_long bss_length;

	/* The length of the symbols section in bytes. */
	sun3_unsigned_long symbol_table_length;

	/*
	 * The address in the .text section at which to start execution.
	 * In a ZMAGIC file on a SPARC this is usuall (always?) 0x2020 since
	 * it is assumed that the a.out file (including the header) will be
	 * loaded at 0x2000 and the extra 0x20 is to account for the header.
	 */
	sun3_unsigned_long start_address;

	/*
	 * The length of the text relocation section.
	 * This should be 0 in a ZMAGIC a.out
	 */
	sun3_unsigned_long text_relocation_length;

	/* 
	 * The length of the data relocation section.
	 * This should be 0 in a ZMAGIC a.out
	 */
	sun3_unsigned_long data_relocation_length;
};

typedef struct sun3_aout_header sun3_aout_header;

/*
 * Only one value for sun3_aout_header.machine_type is accepted by
 * sun3 emulator ...
 */

#define sun3_machine_type_SUN3 0x2

#endif
