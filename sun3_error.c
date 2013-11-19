#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include "sun3_error.h"
#include "sun3_mem.h"

static void sun3_error_delete_strings(sun3_error *e)
{
	size_t i;
	for (i= 0; i < e->cursor; i+=1) {
		sun3_mem_free(e->buffer[i]);
	}
	sun3_mem_free(e->buffer);
}


static void sun3_error_add_string(sun3_error *e, char *str)
{
	assert(e->cursor <= e->size);

	if (e->cursor == e->size) {
		char **buff;
		e->size += 2;
		buff= sun3_mem_realloc(e->buffer, e->size * sizeof(char *));
		if (buff == (char **)0) {
			sun3_error_delete_strings(e);
			e->system_errno= errno;
			e->application_errno= sun3_error_memory;
			return;
		} else {
			e->buffer= buff;
		}
	}
	e->buffer[e->cursor++]= str;
}



void sun3_error_open(sun3_error *e)
{
	e->buffer=            (char **)0;
	e->cursor=            0;
	e->size=              0;
	e->n_error_types=     sun3_error_file_io;
	e->application_errno= 0;
	e->system_errno=      0;
	e->nested_error=      0;
}


size_t sun3_error_allocate_code(sun3_error *e)
{
	return ++e->n_error_types;
}


void sun3_error_close(sun3_error *e)
{
	sun3_error_delete_strings(e);
}



char const *sun3_error_system_message(sun3_error *e)
{
	return strerror(e->system_errno);
}



int sun3_error_code(sun3_error *e)
{
	return e->application_errno;
}



void sun3_error_register_code(sun3_error *e, size_t code)
{
	if (e->application_errno == sun3_error_none) {
		e->system_errno= errno;
		e->application_errno= code;
	} else {
		e->nested_error= 1;
	}
}


void sun3_error_override_code(sun3_error *e, int code)
{
	assert(e->application_errno != sun3_error_none);
	e->application_errno= code;
}


void sun3_error_register_message(sun3_error *e, char const *msg)
{
	assert(e->application_errno != sun3_error_none);
	if (!e->nested_error) {
		size_t   l= strlen(msg);
		char   * b= sun3_mem_malloc(l+1);
		if (b == (char *)0) {
			e->system_errno= errno;
			e->application_errno= sun3_error_memory;
			e->nested_error= 1;
			sun3_error_delete_strings(e);
		} else {
			sun3_error_add_string(e, strcpy(b, msg));
		}
	}
}


char const *sun3_error_arg(sun3_error *e, size_t n)
{
	assert(n < e->cursor);
	return e->buffer[n];
}

