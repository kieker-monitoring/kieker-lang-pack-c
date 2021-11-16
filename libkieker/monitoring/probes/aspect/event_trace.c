#include "event_trace.h"

#include <unistd.h>
#include <pthread.h>

#include "../../utilities/kieker_error.h"
#include "../../../include/kieker_controller.h"
#include "../../controller/kieker_adaptive_monitoring.h"
#include "../kieker_probe_events.h"

/*
 * init the event aspect
 */
void kieker_probe_aspect_event_initialize() {
	kieker_controller_initialize();
    int ret = kieker_adaptive_load_exclude_file("exclude_classes.txt");
    if (ret < 0) {
        exit(-1);
    }
}

/*
 * sends the monitoring data before the call of the monitored function
 */
kieker_trace_t* kieker_probe_aspect_event_before_operation(char *operation_signature, char *class_signature) {
    /* get trace */
    kieker_trace_t* trace = kieker_trace_get();
	int position = 0;

    /* new created trace */
    if (trace->order_index == -1) {
		position = kieker_probe_create_trace_metadata(position, trace);
    }

    trace->stack++;

    /* only send monitoring data if MAX_CALL_DEPTH is not reached and
     * component is not excluded from monitoring
     */
    if (!kieker_adaptive_is_class_excluded(class_signature)) {
        trace->order_index++;
    	position = kieker_probe_create_before_operation_event(position, trace, class_signature, operation_signature);

    	kieker_controller_send(position);
    }

    return trace;
}

/*
 * sends the monitoring data after the call of the monitored function
 */
void kieker_probe_aspect_event_after_operation(kieker_trace_t *trace, char *operation_signature, char *class_signature) {

    /* only send monitoring data if MAX_CALL_DEPTH is not reached and
     * component is not excluded from monitoring
     */
    if (!kieker_adaptive_is_class_excluded(class_signature)) {
    	int position = 0;
    	position = kieker_probe_create_after_operation_event(position, trace, class_signature, operation_signature);

    	trace->order_index++;

    	kieker_controller_send(position);

    }

    trace->stack--;

    /* detect end of trace */
    if (trace->stack == -1) {
        kieker_trace_remove(trace);
    }
}

/*
 * finalize event aspect
 */
void kieker_probe_aspect_event_finalize() {
    kieker_controller_finalize();
}


