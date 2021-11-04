#include "../../../libkieker/common/utilities/kieker_error.h"

/*
 * print error message, use KIEKER_ERROR macro for better usabilty
 */
void kieker_error(char* error_message, char* file, int line) {
    printf(ANSI_COLOR_RED "KIEKER ERROR in %s at line %i: %s" ANSI_COLOR_RESET "\n", file, line, error_message);
}
