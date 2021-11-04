#include "../../libkieker/monitoring/probes/aspect/event_aspect.h"

#include <unistd.h>
#include <pthread.h>

#include "../../../common/utilities/kieker_error.h"
#include "../../../common/utilities/measure_time.h"
#include "exclude_classes.h"

#define MAX_CALL_DEPTH 100


char hostname[128];
int sockfd;

/*
 * init the event aspect
 */
void init_event_aspect() {
    init_controller();
    gethostname(hostname, 128);
    sockfd = get_socket();
    int ret = load_exclude_file("exclude_classes.txt");
    if (ret < 0) {
        exit(-1);
    }
}

/*
 * sends the monitoring data before the call of the monitored function
 */
trace_hash_t* before_event_aspect(char *operationSignature, char *classSignature) {
    int thread_id = (int) pthread_self();

    /* get trace */
    trace_hash_t* data = get_trace(thread_id);
    trace_t* trace = data->trace;

    /* new created trace */
    if (trace->eoi == -1) {
        /* send meta data */
        TraceMetaData meta;
        meta.traceId = trace->trace_id;
        meta.threadId = thread_id;
        meta.sessionId = "<no session>";
        meta.hostname = hostname;
        meta.parentTraceId = -1;
        meta.parentOrderId = -1;

        send_trace_meta_data(&meta, trace->buffer);
    }

    trace->ess++;

    /* only send monitoring data if MAX_CALL_DEPTH is not reached and
     * component is not excluded from monitoring
     */
    if (trace->ess < MAX_CALL_DEPTH && !is_class_excluded(classSignature)) {
        trace->eoi++;

        OperationEvent event;
        event.timestamp = measure_time();
        event.traceId = trace->trace_id;
        event.orderIndex = trace->eoi;
        event.operationSignature = operationSignature;
        event.classSignature = classSignature;

        send_before_event(&event, trace->buffer);
    }

    return data;
}

/*
 * sends the monitoring data aftere the call of the monitored function
 */
void after_event_aspect(trace_hash_t *data, char *operationSignature, char *classSignature) {
    trace_t* trace = data->trace;

    /* only send monitoring data if MAX_CALL_DEPTH is not reached and
     * component is not excluded from monitoring
     */
    if (trace->ess < MAX_CALL_DEPTH && !is_class_excluded(classSignature)) {
        trace->eoi++;

        OperationEvent event;
        event.timestamp = measure_time();
        event.traceId = trace->trace_id;
        event.orderIndex = trace->eoi;
        event.operationSignature = operationSignature;
        event.classSignature = classSignature;

        send_after_event(&event, trace->buffer);
    }

    trace->ess--;

    /* detect end of trace */
    if (data->trace->ess == -1) {
        remove_trace(data);
	/* reset trace object */
	//data->trace->eoi = -1;
    }
}

/*
 * deinit event aspect
 */
void deinit_event_aspect() {
    deinit_controller();
}

void send_before_event(const OperationEvent *event, char* buffer) {
    int length = before_operation_event_serialize(buffer, 0, event);

    int ret = write(sockfd, buffer, length);
    if (ret < 0)
        KIEKER_ERROR("Could not write to socket!");
}

void send_after_event(const OperationEvent *event, char* buffer) {
    int length = after_operation_event_serialize(buffer, 0, event);

    int ret = write(sockfd, buffer, length);
    if (ret < 0)
        KIEKER_ERROR("Could not write to socket!");
}

void send_trace_meta_data(const TraceMetaData *meta, char* buffer) {
    int length = trace_meta_data_serialize(buffer, 40, 0, meta);

    int ret = write(sockfd, buffer, length);
    if (ret < 0)
        KIEKER_ERROR("Could not write to socket!");
}
