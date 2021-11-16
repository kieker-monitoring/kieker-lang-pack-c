/*
 * kieker_probe_plain.c
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#include "kieker_probe_plain.h"

#include <stdio.h>
#include <kieker_controller.h>
#include "kieker_probe_events.h"

void kieker_probe_before_operation_event(const char* class_signature, const char* operation_signature) {
	int position = 0;

	kieker_trace_t* trace = kieker_trace_get();

	if (trace->order_index == -1) { // new thread
		position = kieker_probe_create_trace_metadata(position, trace);
	}

	trace->order_index++;
	trace->stack++;

	position = kieker_probe_create_before_operation_event(position, trace, class_signature, operation_signature);

	kieker_controller_send(position);
}

void kieker_probe_after_operation_event(const char* class_signature, const char* operation_signature) {
	kieker_trace_t* trace = kieker_trace_get();

	int position = 0;
	position = kieker_probe_create_after_operation_event(position, trace, class_signature ,operation_signature);

	trace->order_index++;
	trace->stack--;

	kieker_controller_send(position);

	if (trace->stack == 0) {
		kieker_trace_remove(trace);
	}
}
