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


void kieker_probe_before_operation_record(const char* class_signature, const char* operation_signature) {
	kieker_common_record_flow_trace_operation_before_operation_event event;
	int offset = 0;

	kieker_thread_array_entry* entry = kieker_controller_get_thread_entry();

	if (entry->order_index == 0) { // new thread
		kieker_common_record_flow_trace_trace_metadata trace_metadata;

		trace_metadata.hostname = kieker_controller_get_hostname();
		trace_metadata.nextOrderId = 0;
		trace_metadata.parentOrderId = 0;
		trace_metadata.sessionId = "<no-session>";
		trace_metadata.threadId = entry->thread_id;
		trace_metadata.traceId = entry->trace_id;

		offset += kieker_common_record_flow_trace_trace_metadata_serialize(kieker_controller_get_buffer(),
				KIEKER_FLOW_TRACE_METADATA, offset, trace_metadata);
	}

	event.timestamp = kieker_controller_get_time_ms();
	event.traceId = kieker_controller_get_trace_id(entry);
	event.classSignature = class_signature;
	event.operationSignature = operation_signature;
	event.orderIndex = entry->order_index;

	offset += kieker_common_record_flow_trace_operation_before_operation_event_serialize(kieker_controller_get_buffer(),
			KIEKER_FLOW_BEFORE_OPERATION, offset, event);

	kieker_controller_send(offset);
}

void kieker_probe_after_operation_record(const char* class_signature, const char* operation_signature) {
	kieker_common_record_flow_trace_operation_after_operation_event event;

	kieker_thread_array_entry* entry = kieker_controller_get_thread_entry();

	event.timestamp = kieker_controller_get_time_ms();
	event.traceId = kieker_controller_get_trace_id(entry);
	event.classSignature = class_signature;
	event.operationSignature = operation_signature;
	event.orderIndex = entry->order_index;

	int offset = kieker_common_record_flow_trace_operation_after_operation_event_serialize(kieker_controller_get_buffer(),
			KIEKER_FLOW_AFTER_OPERATION, 0, event);

	kieker_controller_send(offset);
}
