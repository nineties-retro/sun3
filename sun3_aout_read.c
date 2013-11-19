/*
 * Supports the reading of Sun3 [NOZ]MAGIC a.out binaries.
 */

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_machine_types.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_memory_io.h"
#include "sun3_aout.h"
#include "sun3_aout_read.h"
#include "sun3_aout_magic.h"

struct sun3_aout_read_header {
	sun3_memory_address text_start;
	sun3_unsigned_long  text_length;
	sun3_memory_address data_start;
	sun3_unsigned_long  data_length;
	sun3_unsigned_long  bss_length;
	sun3_unsigned_long  start_address;
	sun3_unsigned_word  magic_value;
	sun3_unsigned_byte  machine_type;
	char const         *image_file_name;
	sun3_memory_file    image_file;
	sun3_errors        *errors;
};

typedef struct sun3_aout_read_header sun3_aout_read_header;


static int sun3_aout_read_header_open(char const *image_file_name,
				      struct sun3_memory *memory,
				      struct sun3_errors *errors,
				      struct sun3_aout_read_header *header)
{
	header->errors= errors;
	header->image_file_name= image_file_name;
	if (!sun3_memory_io_open(image_file_name, &header->image_file)) {
		sun3_error_register_code(errors->handler, sun3_error_file_io);
		sun3_error_register_message(errors->handler, image_file_name);
		sun3_error_register_message(errors->handler, "open");
		goto could_not_open_image_file;
	}
	memory->write_start= memory->read_start= sun3_memory_page_size;
	if (sun3_memory_io_read(header->image_file, memory, sun3_memory_page_size, sizeof(sun3_aout_header)) != sizeof(sun3_aout_header)) {
		sun3_error_register_code(errors->handler, sun3_error_file_io);
		sun3_error_register_message(errors->handler, image_file_name);
		sun3_error_register_message(errors->handler, "read");
		goto could_not_read_aout_header;
	}
	
	header->magic_value = sun3_memory_read_unsigned_word(memory, sun3_memory_page_size+offsetof(sun3_aout_header, magic));
	header->text_length = sun3_memory_read_unsigned_long(memory, sun3_memory_page_size+offsetof(sun3_aout_header, text_length));
	header->data_length = sun3_memory_read_unsigned_long(memory, sun3_memory_page_size+offsetof(sun3_aout_header, data_length));
	header->bss_length = sun3_memory_read_unsigned_long(memory, sun3_memory_page_size+offsetof(sun3_aout_header, bss_length));
	header->machine_type = sun3_memory_read_unsigned_byte(memory, sun3_memory_page_size+offsetof(sun3_aout_header, machine_type));
	header->start_address = sun3_memory_read_unsigned_long(memory, sun3_memory_page_size+offsetof(sun3_aout_header, start_address));
	return 1;

could_not_read_aout_header:
	(void)sun3_memory_io_close(header->image_file);
could_not_open_image_file:
	return 0;
}



static int sun3_aout_read_header_close(struct sun3_aout_read_header *header)
{
	if (!sun3_memory_io_close(header->image_file)) {
		sun3_error_register_code(header->errors->handler, sun3_error_file_io);
		sun3_error_register_message(header->errors->handler, header->image_file_name);
		sun3_error_register_message(header->errors->handler, "close");
		return 0;
	}
	return 1;
}



static int sun3_aout_validate_magic(struct sun3_aout_read_header *header)
{
	switch(header->magic_value) {
	case sun3_aout_ZMAGIC:
	case sun3_aout_OMAGIC:
	case sun3_aout_NMAGIC:
		return 1;
	default:
		sun3_error_register_code(header->errors->handler, header->errors->unsupported_aout_magic_value);
		sun3_error_register_message(header->errors->handler, header->image_file_name);
		return 0;
	}
}


static int sun3_aout_validate_machine_type(struct sun3_aout_read_header *header)
{
	if (header->machine_type != sun3_machine_type_SUN3) {
		sun3_error_register_code(header->errors->handler, header->errors->not_sun3_image);
		sun3_error_register_message(header->errors->handler, header->image_file_name);
		return 0;
	}
	return 1;
}


static int sun3_aout_read_text(struct sun3_aout_read_header *header,
			       struct sun3_memory *memory)
{
	unsigned int text_offset= (header->magic_value == sun3_aout_ZMAGIC)
		? sizeof(sun3_aout_header)
		: 0;
	header->text_start= sun3_memory_page_size+text_offset;
	
	if (sun3_memory_io_read(header->image_file, memory, header->text_start, header->text_length) != (int)header->text_length) {
		sun3_error_register_code(header->errors->handler, sun3_error_file_io);
		sun3_error_register_message(header->errors->handler, header->image_file_name);
		sun3_error_register_message(header->errors->handler, "read");
		return 0;
	}
	return 1;
}



static int sun3_aout_read_data(struct sun3_aout_read_header *header, 
			       struct sun3_memory *memory)
{
	unsigned int n_text_segments= 1 + (header->text_start+header->text_length-1)/sun3_memory_segment_size;
	header->data_start= n_text_segments*sun3_memory_segment_size;
	if (header->data_length != 0) {
		int nread= sun3_memory_io_read(header->image_file, memory, header->data_start, header->data_length);
		if (nread != (int)header->data_length) {
			sun3_error_register_code(header->errors->handler, sun3_error_file_io);
			sun3_error_register_message(header->errors->handler, header->image_file_name);
			sun3_error_register_message(header->errors->handler, "read");
			return 0;
		}
	}
	memory->write_start= (header->magic_value == sun3_aout_OMAGIC)
		? sun3_memory_page_size
		: header->data_start;
	return 1;
}



static void sun3_aout_read_bss(struct sun3_aout_read_header *header,
			       struct sun3_memory *memory)
{
	sun3_memory_bss(memory, header->data_start+header->data_length, header->bss_length);
}



int sun3_aout_read(char const *image_file_name,
		   struct sun3_memory * memory,
		   struct sun3_errors * errors,
		   sun3_memory_address * start_address)
{
	sun3_aout_read_header header;
	if (!sun3_aout_read_header_open(image_file_name, memory, errors, &header))
		goto could_not_open_header;

	if (!sun3_aout_validate_magic(&header))
		goto unsupported_aout_magic_value;

	if (!sun3_aout_validate_machine_type(&header))
		goto not_a_sun3_image;
	
	if (!sun3_aout_read_text(&header, memory))
		goto could_not_read_text;

	if (!sun3_aout_read_data(&header, memory))
		goto could_not_read_data;

	sun3_aout_read_bss(&header, memory);
	
	*start_address= header.start_address;
	/*
	 * Could check that this address is within the text section and report
	 * an error if not.  However, prefer to let the MMU detect any error
	 * when an attempt is made to fetch the instruction.
	 */

	if (!sun3_aout_read_header_close(&header))
		goto could_not_close_header;
	return 1;

could_not_read_data:
could_not_read_text:
not_a_sun3_image:
unsupported_aout_magic_value:
	sun3_aout_read_header_close(&header);
could_not_close_header:
could_not_open_header:
	return 0;
}
