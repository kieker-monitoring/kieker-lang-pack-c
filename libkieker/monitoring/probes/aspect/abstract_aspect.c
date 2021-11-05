#include "../../libkieker/monitoring/probes/aspect/abstract_aspect.h"

#include <pthread.h>
#include <unistd.h>

#include "../../utilities/kieker_error.h"
#include "../../utilities/measure_time.h"
#include "../../../include/kieker_controller.h"
#include "../../controller/kieker_trace.h"

void init_aspect() {
	kieker_controller_initialize();
}

kieker_trace_t* before_aspect(kieker_common_record_controlflow_operation_execution_record* record) {
    /* get thread unique identifier */
    int thread_id = (int) pthread_self();

    kieker_trace_t* trace = kieker_trace_get(thread_id);

    /* increment indexes */
    trace->order_index++;
    trace->stack++;

    /* set record data */
    record->hostname = kieker_controller_get_hostname();;
    record->sessionId = "<no session>";
    record->traceId = trace->trace_id;
    record->eoi = trace->order_index;
    record->ess = trace->stack;

    /* measure entry time */
    record->tin = measure_time();

    return trace;
}

void after_aspect(kieker_trace_t *trace, kieker_common_record_controlflow_operation_execution_record *record) {
    /* measure exit time */
    record->tout = measure_time();

    /* decrement execution stack index */
    trace->stack--;
}

void after_aspect2(kieker_trace_t *trace, kieker_common_record_controlflow_operation_execution_record *record) {
    send_record(record, trace->buffer);

    /* detect end of trace */
    if (trace->stack == -1) {
        kieker_trace_remove(trace);
    }
}

void deinit_aspect() {
    kieker_controller_finalize();
}

void send_record(const kieker_common_record_controlflow_operation_execution_record *record, char* buffer) {
    int length = kieker_common_record_controlflow_operation_execution_record_serialize(buffer, 0, *record);

    kieker_controller_send(length);
}
