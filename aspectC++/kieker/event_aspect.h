#ifndef EVENT_ASPECT_H
#define EVENT_ASPECT_H

#include <controller.h>
#include <Trace/trace.h>
#include <Record/operation_event.h>

void init_event_aspect();
trace_hash_t* before_event_aspect(char *operationSignature, char *classSignature);
void after_event_aspect(trace_hash_t *data, char *operationSignature, char *classSignature);
void deinit_event_aspect();

void send_before_event(const OperationEvent *event, char* buffer);
void send_after_event(const OperationEvent *event, char* buffer);
void send_trace_meta_data(const TraceMetaData *meta, char* buffer);

#endif
