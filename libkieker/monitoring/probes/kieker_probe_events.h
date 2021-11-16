#ifndef _KIEKER_PROBE_EVENTS_H
#define _KIEKER_PROBE_EVENTS_H

#include "../controller/kieker_trace.h"

#define KIEKER_FLOW_TRACE_METADATA 0
#define KIEKER_FLOW_BEFORE_OPERATION 1
#define KIEKER_FLOW_AFTER_OPERATION 2

/*
 * Create and store a trace metadata record in the write buffer starting at position.
 *
 * @param position buffer position
 * @param trace trace information
 */
int kieker_probe_create_trace_metadata(int position, kieker_trace_t* trace);

/*
 * Create and store a before operation event in the wirt buffer starting at position.
 *
 * @param position buffer position
 * @param trace current active trace
 * @param class_signature
 * @param operation_signature
 */
int kieker_probe_create_before_operation_event(int position, kieker_trace_t* trace, const char* class_signature, const char* operation_signature);

/*
 * Create and store a after operation event in the wirt buffer starting at position.
 *
 * @param position buffer position
 * @param trace current active trace
 * @param class_signature
 * @param operation_signature
 */
int kieker_probe_create_after_operation_event(int position, kieker_trace_t* trace, const char* class_signature, const char* operation_signature);

#endif
