#include "../../libkieker/monitoring/probes/aspect/event_aspect.h"

#include <unistd.h>
#include <pthread.h>

#include "../../utilities/kieker_error.h"
#include "../../utilities/measure_time.h"
#include "../../../include/kieker_controller.h"
#include "../../controller/kieker_adaptive_monitoring.h"

/*
 * init the event aspect
 */
void init_event_aspect() {
	kieker_controller_initialize();
    int ret = kieker_adaptive_load_exclude_file("exclude_classes.txt");
    if (ret < 0) {
        exit(-1);
    }
}

/*
 * sends the monitoring data before the call of the monitored function
 */
kieker_trace_hash_t* before_event_aspect(char *operationSignature, char *classSignature) {
    /* get trace */
    kieker_trace_hash_t* trace = kieker_trace_get();

    /* new created trace */
    if (trace->order_index == -1) {
        /* send meta data */
    	kieker_common_record_flow_trace_trace_metadata meta;
        meta.traceId = trace->trace_id;
        meta.threadId = trace->thread_id;
        meta.sessionId = "<no session>";
        meta.hostname = kieker_controller_get_hostname();
        meta.parentTraceId = -1;
        meta.parentOrderId = -1;

        send_trace_meta_data(meta, trace->buffer);
    }

    trace->stack++;

    /* only send monitoring data if MAX_CALL_DEPTH is not reached and
     * component is not excluded from monitoring
     */
    if (!kieker_adaptive_is_class_excluded(classSignature)) {
        trace->order_index++;

        kieker_common_record_flow_trace_operation_before_operation_event event;
        event.timestamp = measure_time();
        event.traceId = trace->trace_id;
        event.orderIndex = trace->order_index;
        event.operationSignature = operationSignature;
        event.classSignature = classSignature;

        send_before_event(event, trace->buffer);
    }

    return trace;
}

/*
 * sends the monitoring data after the call of the monitored function
 */
void after_event_aspect(kieker_trace_hash_t *trace, char *operationSignature, char *classSignature) {

    /* only send monitoring data if MAX_CALL_DEPTH is not reached and
     * component is not excluded from monitoring
     */
    if (!kieker_adaptive_is_class_excluded(classSignature)) {
        trace->order_index++;

        kieker_common_record_flow_trace_operation_after_operation_event event;
        event.timestamp = measure_time();
        event.traceId = trace->trace_id;
        event.orderIndex = trace->order_index;
        event.operationSignature = operationSignature;
        event.classSignature = classSignature;

        send_after_event(event, trace->buffer);
    }

    trace->stack--;

    /* detect end of trace */
    if (trace->stack == -1) {
        kieker_trace_remove(trace);
	/* reset trace object */
	//data->trace->eoi = -1;
    }
}

/*
 * deinit event aspect
 */
void deinit_event_aspect() {
    kieker_controller_finalize();
}

void send_before_event(const kieker_common_record_flow_trace_operation_before_operation_event event, char* buffer) {
    int length = kieker_common_record_flow_trace_operation_before_operation_event_serialize(buffer, 0, event);

    kieker_controller_send(length);
}

void send_after_event(const kieker_common_record_flow_trace_operation_after_operation_event event, char* buffer) {
    int length = kieker_common_record_flow_trace_operation_after_operation_event_serialize(kieker_controller_get_buffer(), 0, event);

    kieker_controller_send(length);
}

void send_trace_meta_data(const kieker_common_record_flow_trace_trace_metadata meta, char* buffer) {
    int length = kieker_common_record_flow_trace_trace_metadata_serialize(buffer, 40, meta);

    kieker_controller_send(length);
}
