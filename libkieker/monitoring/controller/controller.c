#include "../../libkieker/monitoring/controller/controller.h"

#include <stdio.h>
#include <pthread.h>

#include <Util/socket.h>

#include "../../libkieker/common/utilities/kieker_error.h"


#define HOST "localhost"
//#define HOST "192.168.48.227"
#define PORT 9000

int initialized = 0;
int sockfd;

pthread_rwlock_t traces_hash_lock;

/*
 * init monitoring controller
 */
void init_controller() {
    if (!initialized) {
        sockfd = socket_open_write (HOST, PORT);
        if (sockfd < 0) {
            KIEKER_ERROR("Could not open socket!");
            exit(-1);
        }

        init_trace_id();

        int ret = pthread_rwlock_init(&traces_hash_lock, NULL);
        if (ret != 0)
            KIEKER_ERROR("Could not create rwlock!");

        print_kieker_config();

        initialized = 1;
    }
}

/*
 * get the trace object for the current thread.
 * If the trace object does not exist, it will be created.
 */
trace_hash_t* get_trace(int thread_id) {
    /* get trace data */
    int ret = pthread_rwlock_rdlock(&traces_hash_lock);
    if (ret != 0)
        KIEKER_ERROR("Could not aquire rwlock!");
    trace_hash_t* data = find_trace(thread_id);
    pthread_rwlock_unlock(&traces_hash_lock);

    if (data == NULL) {
        /* create new trace data */
        trace_t* trace = (trace_t*) malloc(sizeof(trace_t));
        long long trace_id = get_next_trace_id();
        trace_create(trace, trace_id);

        data = (trace_hash_t*) malloc(sizeof(trace_hash_t));
        data->thread_id = thread_id;
        data->trace = trace;

        ret = pthread_rwlock_wrlock(&traces_hash_lock);
        if (ret != 0)
            KIEKER_ERROR("Could not aquire rwlock!");
        add_trace(data);
        pthread_rwlock_unlock(&traces_hash_lock);
    }

    return data;
}

/*
 * removes the trace object
 */
void remove_trace(trace_hash_t *data) {
    int ret = pthread_rwlock_wrlock(&traces_hash_lock);
    if (ret != 0)
        KIEKER_ERROR("Could not aquire rwlock");
    delete_trace(data);
    pthread_rwlock_unlock(&traces_hash_lock);

    trace_destroy(data->trace);
    free(data->trace);
    free(data);
}

/*
 * deinit controller
 */
void deinit_controller() {
    socket_close(sockfd);
    pthread_rwlock_destroy(&traces_hash_lock);
}

/*
 * returns the socket id
 */
int get_socket() {
    return sockfd;
}

/*
 * prints a short kieker config message
 */
void print_kieker_config() {
    printf("KIEKER config:\n");
    printf("\tsend data to: %s\n", HOST);
    printf("\tusing port: %i\n", PORT);
}
