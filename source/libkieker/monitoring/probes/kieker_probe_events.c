#include "kieker_probe_events.h"

#include "../../common/record/flow/trace/operation/before_operation_event.h"
#include "../../common/record/flow/trace/operation/after_operation_event.h"
#include "../../common/record/flow/trace/trace_metadata.h"
#include <kieker_controller.h>


int kieker_probe_create_trace_metadata(int position, kieker_trace_t* trace) {
	kieker_common_record_flow_trace_trace_metadata trace_metadata;

	trace_metadata.hostname = kieker_controller_get_hostname();
	trace_metadata.nextOrderId = 1;
	trace_metadata.parentOrderId = 0;
	trace_metadata.sessionId = "<no-session>";
	trace_metadata.threadId = trace->thread_id;
	trace_metadata.traceId = trace->trace_id;
	trace_metadata.parentOrderId = 0;
	trace_metadata.parentTraceId = -1;

	position = kieker_controller_prefix_serialize(KIEKER_FLOW_TRACE_METADATA, position);
	return kieker_common_record_flow_trace_trace_metadata_serialize(kieker_controller_get_buffer(), position, trace_metadata);
}

int kieker_probe_create_before_operation_event(int position, kieker_trace_t* trace, const char* class_signature, const char* operation_signature) {
	kieker_common_record_flow_trace_operation_before_operation_event event;

	event.timestamp = kieker_controller_get_time_ms();
	event.traceId = trace->trace_id;
	event.classSignature = class_signature;
	event.operationSignature = operation_signature;
	event.orderIndex = trace->order_index;

	position = kieker_controller_prefix_serialize(KIEKER_FLOW_BEFORE_OPERATION, position);
	return kieker_common_record_flow_trace_operation_before_operation_event_serialize(kieker_controller_get_buffer(), position, event);
}

int kieker_probe_create_after_operation_event(int position, kieker_trace_t* trace, const char* class_signature, const char* operation_signature) {
	kieker_common_record_flow_trace_operation_after_operation_event event;

	event.timestamp = kieker_controller_get_time_ms();
	event.traceId = trace->trace_id;
	event.classSignature = class_signature;
	event.operationSignature = operation_signature;
	event.orderIndex = trace->order_index;

	position = kieker_controller_prefix_serialize(KIEKER_FLOW_AFTER_OPERATION, position);
	return kieker_common_record_flow_trace_operation_after_operation_event_serialize(kieker_controller_get_buffer(), position, event);
}
