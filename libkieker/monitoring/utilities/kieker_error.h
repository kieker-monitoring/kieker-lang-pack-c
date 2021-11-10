#ifndef _KIEKER_ERROR_H_
#define _KIEKER_ERROR_H_

#include <stdio.h>

void kieker_error_print(char* file, int line, char* error_message, ...);

#define KIEKER_ERROR_PLAIN(s) fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__,s)

#define KIEKER_ERROR(s, ...) fprintf(stderr, "%s:%d", __FILE__, __LINE__); fprintf(stderr, s, __VA_ARGS__)

#endif
