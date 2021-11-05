#include <time.h>
#include <pthread.h>

#include "kieker_trace.h"
#include "../utilities/kieker_error.h"

/*
 * holds the hashmap of trace data
 */
kieker_trace_hash_t* kieker_traces = NULL;

extern pthread_rwlock_t kieker_controller_hash_lock;

void kieker_trace_create(kieker_trace_hash_t* trace, int thread_id, long long trace_id);
kieker_trace_hash_t *kieker_trace_find(int thread_id);
void kieker_trace_delete(kieker_trace_hash_t *data);

/** trace id generation */
long long kieker_trace_get_next_id();

/*
 * get the trace object for the current thread.
 * If the trace object does not exist, it will be created.
 */
kieker_trace_hash_t* kieker_trace_get() {
	 int thread_id = (int) pthread_self();
    /* get trace data */
    int ret = pthread_rwlock_rdlock(&kieker_controller_hash_lock);
    if (ret != 0)
        KIEKER_ERROR("Could not acquire rwlock!");
    kieker_trace_hash_t* data = kieker_trace_find(thread_id);
    pthread_rwlock_unlock(&kieker_controller_hash_lock);

    if (data == NULL) {
        /* create new trace data */
        data = (kieker_trace_hash_t*) malloc(sizeof(kieker_trace_hash_t));
        kieker_trace_create(data, thread_id, kieker_trace_get_next_id());

        ret = pthread_rwlock_wrlock(&kieker_controller_hash_lock);
        if (ret != 0)
            KIEKER_ERROR("Could not acquire rwlock!");
        HASH_ADD_INT(kieker_traces, thread_id, data);
        pthread_rwlock_unlock(&kieker_controller_hash_lock);
    }

    return data;
}

/*
 * removes the trace object
 */
void kieker_trace_remove(kieker_trace_hash_t *data) {
    int ret = pthread_rwlock_wrlock(&kieker_controller_hash_lock);
    if (ret != 0)
        KIEKER_ERROR("Could not acquire rwlock");
    kieker_trace_delete(data);
    pthread_rwlock_unlock(&kieker_controller_hash_lock);

    free(data->buffer);
    free(data);
}

/*
 * find a trace object in hashmap
 *
 * return the object on success, otherwise NULL
 */
kieker_trace_hash_t *kieker_trace_find(int thread_id) {
	kieker_trace_hash_t *data;

    HASH_FIND_INT(kieker_traces, &thread_id, data);
    return data;
}

/*
 * remove trace object in hashmap
 */
void kieker_trace_delete(kieker_trace_hash_t *data) {
    HASH_DEL(kieker_traces, data);
}

/*
 * creates a new trace
 */
void kieker_trace_create(kieker_trace_hash_t* trace, int thread_id, long long trace_id) {
    trace->trace_id = trace_id;
    trace->thread_id = thread_id,
    trace->order_index = -1;
    trace->stack = -1;
    trace->buffer = (char*) malloc(1024 * sizeof(char));
}

/*
 * a trace id counter, which will be incremented with each call of get_next_trace_id()
 */
long long kieker_trace_current_id = 0;

/*
 * create initial random trace id
 */
void kieker_trace_init_id() {
    /* initialize random seed: */
    srand(time(NULL));

    long long val = rand() % 1000000000;
    kieker_trace_current_id = (val << 8);
}

/*
 * get new trace id
 */
long long kieker_trace_get_next_id() {
    return kieker_trace_current_id++;
}
