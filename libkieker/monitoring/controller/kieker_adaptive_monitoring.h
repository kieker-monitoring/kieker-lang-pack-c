#ifndef KIEKER_ADAPTIVE_MONITORING_H
#define KIEKER_ADAPTIVE_MONITORING_H

int kieker_adaptive_load_exclude_file(char* fileName);
int kieker_adaptive_is_class_excluded(char* classSignature);
unsigned long kieker_adaptive_exclude_hash(unsigned char *str);
static int kieker_adaptive_comp_ulong(const void *m1, const void *m2);

#endif
