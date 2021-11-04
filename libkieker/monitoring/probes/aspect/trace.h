#ifndef TRACE_H
#define TRACE_H

#include "../../../libkieker/common/utilities/uthash.h"

/*
 * data type that save all trace relevant data
 */
typedef struct trace {
    long long trace_id;
    long long eoi;
    long long ess;
    char* buffer;
} trace_t;

/*
 * trace hash map type, which is requiered by uthash
 */
typedef struct trace_hash {
    int thread_id;
    trace_t* trace;
    UT_hash_handle hh;
} trace_hash_t;

void add_trace(trace_hash_t *data);
trace_hash_t *find_trace(int thread_id);
void delete_trace(trace_hash_t *data);

void trace_create(trace_t* trace, long long trace_id);
void trace_destroy(trace_t* trace);

/** trace id generation */
void init_trace_id();
long long get_next_trace_id();

#endif
