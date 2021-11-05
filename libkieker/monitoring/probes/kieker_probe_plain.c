/*
 * kieker_probe_plain.c
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#include "kieker_probe_plain.h"

#include <stdio.h>
#include <kieker_controller.h>

#include "../../common/record/flow/trace/operation/before_operation_event.h"
#include "../../common/record/flow/trace/operation/after_operation_event.h"
#include "../../common/record/flow/trace/trace_metadata.h"
#include "../controller/kieker_trace.h"


void kieker_probe_before_operation_record(const char* class_signature, const char* operation_signature) {
	kieker_common_record_flow_trace_operation_before_operation_event event;
	int position = 0;

	kieker_trace_hash_t* entry = kieker_trace_get();

	if (entry->order_index == 0) { // new thread
		kieker_common_record_flow_trace_trace_metadata trace_metadata;

		trace_metadata.hostname = kieker_controller_get_hostname();
		trace_metadata.nextOrderId = 1;
		trace_metadata.parentOrderId = 0;
		trace_metadata.sessionId = "<no-session>";
		trace_metadata.threadId = entry->thread_id;
		trace_metadata.traceId = entry->trace_id;
		trace_metadata.parentOrderId = 0;
		trace_metadata.parentTraceId = -1;

		position = kieker_monitoring_controller_prefix_serialize(KIEKER_FLOW_TRACE_METADATA, position);
		position = kieker_common_record_flow_trace_trace_metadata_serialize(kieker_controller_get_buffer(), position, trace_metadata);
	}

	event.timestamp = kieker_controller_get_time_ms();
	event.traceId = entry->trace_id;
	event.classSignature = class_signature;
	event.operationSignature = operation_signature;
	event.orderIndex = entry->order_index;

	entry->order_index++;
	entry->stack++;

	position = kieker_monitoring_controller_prefix_serialize(KIEKER_FLOW_BEFORE_OPERATION, position);
	position = kieker_common_record_flow_trace_operation_before_operation_event_serialize(kieker_controller_get_buffer(), position, event);

	kieker_controller_send(position);
}

void kieker_probe_after_operation_record(const char* class_signature, const char* operation_signature) {
	kieker_common_record_flow_trace_operation_after_operation_event event;

	kieker_trace_hash_t* entry = kieker_trace_get();

	event.timestamp = kieker_controller_get_time_ms();
	event.traceId = entry->trace_id;
	event.classSignature = class_signature;
	event.operationSignature = operation_signature;
	event.orderIndex = entry->order_index;

	entry->order_index++;
	entry->stack--;

	int position = kieker_monitoring_controller_prefix_serialize(KIEKER_FLOW_AFTER_OPERATION, position);
	position = kieker_common_record_flow_trace_operation_after_operation_event_serialize(kieker_controller_get_buffer(), position, event);

	kieker_controller_send(position);

	if (entry->stack == 0) {
		kieker_trace_remove(entry);
	}
}
