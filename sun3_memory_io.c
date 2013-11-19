#include <assert.h>
#include <stddef.h>
#include "sun3_error.h"
#include "sun3_errors.h"
#include "sun3_memory.h"
#include "sun3_memory_io.h"
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int sun3_memory_io_open(char const *file_name, sun3_memory_file *fid)
{
	return (*fid= open(file_name, O_RDONLY)) != -1;
}



int sun3_memory_io_read(sun3_memory_file fid, sun3_memory *mem, sun3_memory_address addr, size_t len)
{
	assert(sun3_memory_inv(mem));
	if (addr < mem->write_start || addr+len > mem->end) {
		sun3_errors_bus_error(mem->errors);
	}
	return read(fid, mem->data+addr, len);
}



int sun3_memory_io_write(sun3_memory_file fid, sun3_memory *mem, sun3_memory_address addr, size_t len)
{
	assert(sun3_memory_inv(mem));
	if (addr < mem->read_start || addr+len > mem->end) {
		sun3_errors_bus_error(mem->errors);
	}
	return write(fid, mem->data+addr, len);
}



int sun3_memory_io_close(sun3_memory_file fid)
{
	return close(fid) == 0;
}
