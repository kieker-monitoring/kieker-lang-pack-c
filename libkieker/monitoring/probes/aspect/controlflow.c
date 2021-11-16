#include "controlflow.h"

#include <pthread.h>
#include <unistd.h>

#include "../../utilities/kieker_error.h"
#include "../../../include/kieker_controller.h"
#include "../../controller/kieker_trace.h"

void kieker_probe_aspect_controlflow_initialize() {
	kieker_controller_initialize();
}

kieker_trace_t* kieker_probe_aspect_controlflow_before(kieker_common_record_controlflow_operation_execution_record* record) {
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
    record->tin = kieker_controller_get_time_ms();

    return trace;
}

void kieker_probe_aspect_controlflow_after(kieker_trace_t *trace, kieker_common_record_controlflow_operation_execution_record *record) {
    /* measure exit time */
    record->tout = kieker_controller_get_time_ms();

    /* decrement execution stack index */
    trace->stack--;
}

void kieker_probe_aspect_controlflow_after2(kieker_trace_t *trace, kieker_common_record_controlflow_operation_execution_record *record) {
	kieker_probe_aspect_controlflow_send(record, trace->buffer);

    /* detect end of trace */
    if (trace->stack == -1) {
        kieker_trace_remove(trace);
    }
}

void kieker_probe_aspect_controlflow_finalize() {
    kieker_controller_finalize();
}

void kieker_probe_aspect_controlflow_send(const kieker_common_record_controlflow_operation_execution_record *record, char* buffer) {
    int length = kieker_common_record_controlflow_operation_execution_record_serialize(buffer, 0, *record);

    kieker_controller_send(length);
}
