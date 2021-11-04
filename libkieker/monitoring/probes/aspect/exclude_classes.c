#include "../../../libkieker/monitoring/probes/aspect/exclude_classes.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../../../libkieker/common/utilities/kieker_error.h"

#define MAX_NUM_EXCLUDE_FILES 200

/*
 * holds all creates hash-values
 */
unsigned long excludes[MAX_NUM_EXCLUDE_FILES];

/*
 * total number of saved hash values
 */
int excludes_size;

/*
 * exclusion is active flag
 */
int use_excludion;

/*
 * loads a list of components, that shell be included/excluded
 *
 * return true on success, otherwise a negative value
 */
int load_exclude_file(char* fileName) {
    printf("load file: %s\n", fileName);
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
    if (line[read - 1] == '\n') line[read - 1] = '\0';

    if (strcmp(line, "EXCLUDE") == 0) {
        use_excludion = 1;
    } else if (strcmp(line, "INCLUDE") == 0) {
        use_excludion = 0;
    } else {
        KIEKER_ERROR("wrong fileheader!");
        return -3;
    }

    /* read component list line by line */
    while ((read = getline(&line, &len, f)) != -1) {

        /* ignore command lines and empty lines */
        if (line[0] != '#' && line[0] != '\n') {
            /* remove \n at the end of string */
            if (line[read - 1] == '\n') line[read - 1] = '\0';

            printf("%s\n\twith hash: %lu\n", line, hash((unsigned char*) line));

            excludes[i] = hash((unsigned char*) line);

            i++;
            if (i > MAX_NUM_EXCLUDE_FILES) {
                KIEKER_ERROR("Not enouth space to store all exclude classes!");
                return -4;
            }
        }
    }

    free(line);
    printf("%i exclude classes found!\n", i);

    fclose(f);

    /* sort hashes for a more efficient search with bsearch */
    excludes_size = i;
    qsort(excludes, excludes_size, sizeof(unsigned long), comp_ulong);

    /* detect equal hashes */
    int j;
    for (j = 0; j < excludes_size - 1; j++) {
        if (excludes[j] == excludes[j + 1]) {
            KIEKER_ERROR("Two strings have equal hash value!");
            return -5;
        }
    }

    return 0;
}

/*
 * returns true, if the given component shell be excluded of monitoring
 */
int is_class_excluded(char* classSignature) {
    unsigned long l = hash((unsigned char*) classSignature);
    int found = (bsearch(&l, excludes, excludes_size, sizeof(unsigned long), comp_ulong) != NULL);

    return (found && use_excludion) || (!found && !use_excludion);
}

/*
 * creates a hash value for a string
 */
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*
 * compare unsigned long values
 */
static int comp_ulong(const void *m1, const void *m2)
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
