#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_mem.h"

int sun3_memory_inv(sun3_memory *mem)
{
	return (mem != 0)
		&& (mem->errors != 0)
		&  ( ( (mem->data == 0)
		     & (mem->read_start == 0)
		     & (mem->write_start == 0)
		     & (mem->end == 0)
		     )
		   | ( (mem->data != 0)
		     & (mem->end != 0)
		     & (mem->nbytes != 0)
		     )
		   );
}



void sun3_memory_init(sun3_memory *mem, sun3_errors *errors)
{
	mem->data = 0;
	mem->read_start = 0;
	mem->write_start = 0;
	mem->end = 0;
	mem->errors = errors;
	assert(sun3_memory_inv(mem));
}



void sun3_memory_set_size(sun3_memory *mem, size_t nbytes)
{
	assert(nbytes > 0);  
	assert(sun3_memory_inv(mem));
	mem->nbytes = nbytes;
	assert(sun3_memory_inv(mem));
}



int sun3_memory_open(sun3_memory *mem)
{
	assert(sun3_memory_inv(mem));
	if ((mem->data = sun3_mem_malloc(mem->nbytes)) == 0) {
		sun3_error_register_code(mem->errors->handler, sun3_error_memory);
		assert(sun3_memory_inv(mem));
		return 0;
	}
	mem->read_start = mem->write_start= 0;
	mem->end = mem->nbytes;
	assert(sun3_memory_inv(mem));
	return 1;
}



void sun3_memory_bss(sun3_memory *mem, sun3_memory_address start, size_t len)
{
	assert(sun3_memory_inv(mem));
	if (start < mem->write_start || start+len > mem->end)
		sun3_errors_bus_error(mem->errors);
	(void)memset(&mem->data[start], 0, len);
	assert(sun3_memory_inv(mem));
}



void sun3_memory_close(sun3_memory *mem)
{
	assert(sun3_memory_inv(mem));
	sun3_mem_free(mem->data);
	mem->data= 0;
	mem->read_start= mem->write_start= 0;
	mem->end= 0;
	mem->nbytes= 0;
	assert(sun3_memory_inv(mem));
}
