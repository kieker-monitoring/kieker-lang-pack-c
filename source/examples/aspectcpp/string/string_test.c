#include "../../aspectcpp/string/string_test.h"

#include <stdio.h>

void string_test() {
    char s[] = "test2";

    char* cs = CONST_STRING;

    printf("Test-Output\n");
    printf("%s\n", "test");
    printf("%s\n", CONST_STRING);
    printf("%s\n", cs);
    printf("%s\n", s);
}
