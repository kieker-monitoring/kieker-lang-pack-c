#ifndef KIEKER_TRACE_H
#define KIEKER_TRACE_H

#include "../utilities/uthash.h"

/*
 * trace hash map type, which is required by uthash
 */
typedef struct kieker_trace_hash {
    long long trace_id;
    long long order_index;
    long long stack;
    char* buffer;
    int thread_id;
    UT_hash_handle hh;
} kieker_trace_hash_t;

void kieker_trace_init_id();

/**
 * Get the current thread or return a new one.
 */
kieker_trace_hash_t* kieker_trace_get();

/**
 * Remove the specified trace trom the register.
 */
void kieker_trace_remove(kieker_trace_hash_t *data);

#endif
