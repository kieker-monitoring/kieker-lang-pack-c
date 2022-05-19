#ifndef _KIEKER_STRING_H_
#define _KIEKER_STRING_H_

#include <stdio.h>
#include <stdlib.h>

/**
 * Trim a string. The returned pointer may be modified.
 * For freeing up space, the original pointer must be retained.
 */
char *kieker_string_trim(char *string);

#endif
