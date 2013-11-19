#ifndef SUN3_ERROR_H
#define SUN3_ERROR_H

struct sun3_error {
	char   ** buffer;
	size_t    cursor;
	size_t    size;
	size_t    n_error_types;
	int       system_errno;
	size_t    application_errno;
	int       nested_error;
};

enum {
	sun3_error_none,
	sun3_error_memory,
	sun3_error_file_io
};

typedef struct sun3_error sun3_error;

void sun3_error_open(struct sun3_error *);

size_t sun3_error_allocate_code(struct sun3_error *);

void sun3_error_register_message(struct sun3_error *, char const *);

char const *sun3_error_system_message(struct sun3_error *);

void sun3_error_register_code(struct sun3_error *, size_t);

int sun3_error_code(struct sun3_error *);

void sun3_error_override_code(struct sun3_error *, int);

const char *sun3_error_arg(struct sun3_error *, size_t n);

void sun3_error_close(struct sun3_error *);

#endif
