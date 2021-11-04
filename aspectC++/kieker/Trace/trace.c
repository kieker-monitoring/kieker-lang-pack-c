#include <Trace/trace.h>

#include <time.h>

/*
 * holds the hashmap of trace data
 */
trace_hash_t* traces = NULL;

/*
 * add a trace object to hashmap
 */
void add_trace(trace_hash_t *data) {
    HASH_ADD_INT(traces, thread_id, data);
}

/*
 * find a trace object in hashmap
 *
 * return the object on success, otherwise NULL
 */
trace_hash_t *find_trace(int thread_id) {
    trace_hash_t *data;

    HASH_FIND_INT(traces, &thread_id, data);
    return data;
}

/*
 * remove trace object in hashmap
 */
void delete_trace(trace_hash_t *data) {
    HASH_DEL(traces, data);
}

/*
 * creates a new trace
 */
void trace_create(trace_t* trace, long long trace_id) {
    trace->trace_id = trace_id;
    trace->eoi = -1;
    trace->ess = -1;
    trace->buffer = (char*) malloc(1024 * sizeof(char));
}

/*
 * destroys a trace
 */
void trace_destroy(trace_t* trace) {
    free(trace->buffer);
    trace->buffer = NULL;
}

/*
 * a trace id counter, which will be incremented with each call of get_next_trace_id()
 */
long long current_trace_id = 0;

/*
 * create initial random trace id
 */
void init_trace_id() {
    /* initialize random seed: */
    srand(time(NULL));

    long long val = rand() % 1000000000;
    current_trace_id = (val << 8);
}

/*
 * get new trace id
 */
long long get_next_trace_id() {
    return current_trace_id++;
}
