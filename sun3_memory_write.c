#include "assert.h"
#include <stddef.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_write.h"
#include "sun3_machine_types.h"
#include "sun3_memory_alignment.h"

void sun3_memory_write_long(struct sun3_memory *mem, sun3_memory_address addr, sun3_unsigned_long value)
{
	assert(sun3_memory_inv(mem));
	if (!sun3_memory_address_long_aligned(addr)) {
		sun3_errors_address_error(mem->errors);
	} else if ((addr < mem->write_start)
		   || (addr > mem->end-sizeof(sun3_unsigned_long))) {
		sun3_errors_bus_error(mem->errors);
	} else {
		sun3_unsigned_byte * data= mem->data;
		data[addr]=   (sun3_unsigned_byte)(value>>24);
		data[addr+1]= (sun3_unsigned_byte)(value>>16);
		data[addr+2]= (sun3_unsigned_byte)(value>>8);
		data[addr+3]= (sun3_unsigned_byte)(value);
	}
}


void sun3_memory_write_word(struct sun3_memory * mem, sun3_memory_address addr, sun3_unsigned_word value)
{
	assert(sun3_memory_inv(mem));
	if (!sun3_memory_address_word_aligned(addr)) {
		sun3_errors_address_error(mem->errors);
	} else if ((addr < mem->write_start)
		   || (addr > mem->end-sizeof(sun3_unsigned_word))) {
		sun3_errors_bus_error(mem->errors);
	} else {
		sun3_unsigned_byte * data= mem->data;
		data[addr]=   (sun3_unsigned_byte)(value>>8);
		data[addr+1]= (sun3_unsigned_byte)(value);
	}
}


void sun3_memory_write_byte(struct sun3_memory * mem, sun3_memory_address addr, sun3_unsigned_byte value)
{
	assert(sun3_memory_inv(mem));
	if ((addr < mem->write_start)
	    || (addr > mem->end-sizeof(sun3_unsigned_byte))) {
		sun3_errors_bus_error(mem->errors);
	} else {
		sun3_unsigned_byte * data= mem->data;
		data[addr]= (sun3_unsigned_byte)(value);
	}
}
