#ifndef EXCLUDE_CLASSES_H
#define EXCLUDE_CLASSES_H

int load_exclude_file(char* fileName);
int is_class_excluded(char* classSignature);
unsigned long hash(unsigned char *str);
static int comp_ulong(const void *m1, const void *m2);

#endif
