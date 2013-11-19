#include "assert.h"
#include <stddef.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_read.h"
#include "sun3_machine_types.h"
#include "sun3_memory_alignment.h"

sun3_unsigned_long sun3_memory_read_unsigned_long(sun3_memory * mem, sun3_memory_address   addr)
{
	assert(sun3_memory_inv(mem));
	if (!sun3_memory_address_long_aligned(addr)) {
		sun3_errors_address_error(mem->errors);
	} else if (addr < mem->read_start
		   ||  addr > mem->end-sizeof(sun3_unsigned_long)) {
		sun3_errors_bus_error(mem->errors);
	} else {
		sun3_unsigned_byte * data= mem->data;
		return ((sun3_unsigned_long)data[addr]<<24)
			| ((sun3_unsigned_long)data[addr+1]<<16)
			| ((sun3_unsigned_long)data[addr+2]<<8)
			| ((sun3_unsigned_long)data[addr+3]<<0);
	}
}


sun3_signed_long sun3_memory_read_signed_long(sun3_memory *mem, sun3_memory_address addr)
{
	return sun3_memory_read_unsigned_long(mem, addr);
}



sun3_unsigned_word sun3_memory_read_unsigned_word(sun3_memory *mem, sun3_memory_address addr)
{
	assert(sun3_memory_inv(mem));
	if (!sun3_memory_address_word_aligned(addr)) {
		sun3_errors_address_error(mem->errors);
	} else if ((addr < mem->read_start)
		   || (addr > mem->end-sizeof(sun3_unsigned_word))) {
		sun3_errors_bus_error(mem->errors);
	} else {
		sun3_unsigned_byte * data= mem->data;
		return ((sun3_unsigned_word)data[addr]<<8)
			| ((sun3_unsigned_word)data[addr+1]<<0);
	}
}


sun3_signed_word sun3_memory_read_signed_word(sun3_memory *mem, sun3_memory_address addr)
{
	return sun3_memory_read_unsigned_word(mem, addr);
}


sun3_unsigned_byte sun3_memory_read_unsigned_byte(sun3_memory *mem, sun3_memory_address addr)
{
	assert(sun3_memory_inv(mem));
	if ((addr < mem->read_start)
	    || (addr > mem->end-sizeof(sun3_unsigned_byte))) {
		sun3_errors_bus_error(mem->errors);
	} else {
		sun3_unsigned_byte * data= mem->data;
		return data[addr];
	}
}


sun3_signed_byte sun3_memory_read_signed_byte(sun3_memory * mem, sun3_memory_address addr)
{
	return sun3_memory_read_unsigned_byte(mem, addr);
}
