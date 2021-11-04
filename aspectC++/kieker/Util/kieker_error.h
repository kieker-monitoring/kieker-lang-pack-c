#ifndef _KIEKER_ERROR_H_
#define _KIEKER_ERROR_H_

#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void kieker_error(char* error_message, char* file, int line);

#define KIEKER_ERROR(s) \
    kieker_error(s, __FILE__, __LINE__)
	
#endif
