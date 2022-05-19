#include "kieker_string.h"

#include <ctype.h>
#include <string.h>

char *kieker_string_ltrim(char *string)
{
    while(isspace(*string)) string++;
    return string;
}

char *kieker_string_rtrim(char *string)
{
    char* back = string + strlen(string);
    while(isspace(*--back));
    *(back+1) = '\0';
    return string;
}

char *kieker_string_trim(char *string)
{
    return kieker_string_rtrim(kieker_string_ltrim(string));
}
