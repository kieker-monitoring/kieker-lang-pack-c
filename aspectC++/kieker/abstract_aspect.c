#include <abstract_aspect.h>

#include <pthread.h>
#include <unistd.h>

#include <controller.h>
#include <Trace/trace.h>
#include <Util/measure_time.h>
#include <Util/kieker_error.h>

char hostname[128];
int sockfd;

void init_aspect() {
    init_controller();
    gethostname(hostname, 128);
    sockfd = get_socket();
}

trace_hash_t* before_aspect(OperationExecutionRecord* record) {
    /* get thread unique identifier */
    int thread_id = (int) pthread_self();

    trace_t* trace;

    trace_hash_t* data = get_trace(thread_id);

    trace = data->trace;

    /* increment indexes */
    trace->eoi++;
    trace->ess++;

    /* set record data */
    record->hostname = hostname;
    record->sessionId = "<no session>";
    record->traceId = data->trace->trace_id;
    record->eoi = trace->eoi;
    record->ess = trace->ess;

    /* measure entry time */
    record->tin = measure_time();

    return data;
}

void after_aspect(trace_hash_t *data, OperationExecutionRecord *record) {
    /* measure exit time */
    record->tout = measure_time();

    /* decrement execution stack index */
    data->trace->ess--;
}

void after_aspect2(trace_hash_t *data, OperationExecutionRecord* record) {
    send_record(record, data->trace->buffer);

    /* detect end of trace */
    if (data->trace->ess == -1) {
        remove_trace(data);
    }
}

void deinit_aspect() {
    deinit_controller();
}

void send_record(const OperationExecutionRecord *record, char* buffer) {
    int length = operation_execution_record_serialize(buffer, 10, 0, *record);

    int ret = write(sockfd, buffer, length);
    if (ret < 0)
        KIEKER_ERROR("Could not write to socket!");
}
