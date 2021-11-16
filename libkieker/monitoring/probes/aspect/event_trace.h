#ifndef KIEKER_PROBES_ASPECT_EVENT_TRACE_H
#define KIEKER_PROBES_ASPECT_EVENT_TRACE_H

#include "../../../common/record/flow/trace/operation/before_operation_event.h"
#include "../../../common/record/flow/trace/operation/after_operation_event.h"
#include "../../../common/record/flow/trace/trace_metadata.h"
#include "../../controller/kieker_trace.h"

void kieker_probe_aspect_event_initialize();
kieker_trace_t* kieker_probe_aspect_event_before_operation(char *operation_signature, char *class_signature);
void kieker_probe_aspect_event_after_operation(kieker_trace_t *data, char *operation_signature, char *class_signature);
void kieker_probe_aspect_event_finalize();

#endif
