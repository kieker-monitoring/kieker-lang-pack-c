#ifndef EVENT_ASPECT_H
#define EVENT_ASPECT_H

#include "../../../common/record/flow/trace/operation/before_operation_event.h"
#include "../../../common/record/flow/trace/operation/after_operation_event.h"
#include "../../../common/record/flow/trace/trace_metadata.h"
#include "../../controller/kieker_trace.h"

void init_event_aspect();
kieker_trace_t* before_event_aspect(char *operationSignature, char *classSignature);
void after_event_aspect(kieker_trace_t *data, char *operationSignature, char *classSignature);
void deinit_event_aspect();

void send_before_event(const kieker_common_record_flow_trace_operation_before_operation_event event, char* buffer);
void send_after_event(const kieker_common_record_flow_trace_operation_after_operation_event event, char* buffer);
void send_trace_meta_data(const kieker_common_record_flow_trace_trace_metadata meta, char* buffer);

#endif
