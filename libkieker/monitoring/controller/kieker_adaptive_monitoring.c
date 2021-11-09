#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../utilities/kieker_error.h"
#include "kieker_adaptive_monitoring.h"

#define MAX_NUM_FILES 200

char *kieker_adaptive_trim(char *s);
unsigned long kieker_adaptive_hash(unsigned char *str);

/*
 * holds all creates hash-values
 */
unsigned long kieker_adaptive_function_hashes[MAX_NUM_FILES];

/*
 * total number of saved hash values
 */
int kieker_adaptive_excludes_size;

/*
 * exclusion is active flag
 */
int kieker_adaptive_mode;

/*
 * loads a list of components, that shell be included/excluded
 *
 * return true on success, otherwise a negative value
 */
int kieker_adaptive_load_exclude_file(char* fileName) {
    fprintf(stdout, "load file: %s\n", fileName);
    FILE* f = fopen(fileName, "r");

    if (f == NULL) {
        KIEKER_ERROR("Could not open file!");
        return -1;
    }

    size_t len = 256;
    char *line = (char*) malloc((len - 1) * sizeof(char));
    ssize_t read;
    unsigned int i = 0;

    /* read first line to determine the type of the list */
    read = getline(&line, &len, f);
    if (read == -1) {
        KIEKER_ERROR("file is empty!");
        return -2;
    }
    /* remove \n at the end of string */
    if (line[read - 1] == '\n') kieker_adaptive_trim(line);

    if (strcmp(line, "EXCLUDE") == 0) {
        kieker_adaptive_mode = 1;
    } else if (strcmp(line, "INCLUDE") == 0) {
        kieker_adaptive_mode = 0;
    } else {
        KIEKER_ERROR("wrong fileheader!");
        return -3;
    }

    /* read component list line by line */
    while ((read = getline(&line, &len, f)) != -1) {
    	kieker_adaptive_trim(line);
        /* ignore command lines and empty lines */
        if (line[0] != '#' && strlen(line) > 0) {
            printf("%s\n\twith hash: %lu\n", line, kieker_adaptive_hash((unsigned char*) line));

            kieker_adaptive_function_hashes[i] = kieker_adaptive_hash((unsigned char*) line);

            i++;
            if (i > MAX_NUM_FILES) {
                KIEKER_ERROR("Not enouth space to store all exclude classes!");
                return -4;
            }
        }
    }

    free(line);
    printf("%i exclude classes found!\n", i);

    fclose(f);

    /* sort hashes for a more efficient search with bsearch */
    kieker_adaptive_excludes_size = i;
    qsort(kieker_adaptive_function_hashes, kieker_adaptive_excludes_size, sizeof(unsigned long), kieker_adaptive_comp_ulong);

    /* detect equal hashes */
    int j;
    for (j = 0; j < kieker_adaptive_excludes_size - 1; j++) {
        if (kieker_adaptive_function_hashes[j] == kieker_adaptive_function_hashes[j + 1]) {
            KIEKER_ERROR("Two strings have equal hash value!");
            return -5;
        }
    }

    return 0;
}

/*
 * returns true, if the given component shell be excluded of monitoring
 */
int kieker_adaptive_is_class_excluded(char* classSignature) {
    unsigned long l = kieker_adaptive_hash((unsigned char*) classSignature);
    int found = (bsearch(&l, kieker_adaptive_function_hashes, kieker_adaptive_excludes_size, sizeof(unsigned long), kieker_adaptive_comp_ulong) != NULL);

    return (found && kieker_adaptive_mode) || (!found && !kieker_adaptive_mode);
}

/*
 * creates a hash value for a string
 */
unsigned long kieker_adaptive_hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (*str != 0) {
    	c = *str++;
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

/*
 * compare unsigned long values
 */
static int kieker_adaptive_comp_ulong(const void *m1, const void *m2)
{
    unsigned long *l1 = (unsigned long*) m1;
    unsigned long *l2 = (unsigned long*) m2;

    if (*l1 < *l2)
        return -1;
    else if (*l1 == *l2)
        return 0;
    else
        return 1;
}



char *kieker_adaptive_ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *kieker_adaptive_rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *kieker_adaptive_trim(char *s)
{
    return kieker_adaptive_rtrim(kieker_adaptive_ltrim(s));
}

