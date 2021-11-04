#ifndef EVENT_ASPECT_H
#define EVENT_ASPECT_H

#include <common/record/flow/trace/operation/before_operation_event.h>
#include <common/record/flow/trace/operation/after_operation_event.h>
#include <common/record/flow/trace/trace_metadata.h>
#include "../../controller/controller.h"
#include "trace.h"

void init_event_aspect();
trace_hash_t* before_event_aspect(char *operationSignature, char *classSignature);
void after_event_aspect(trace_hash_t *data, char *operationSignature, char *classSignature);
void deinit_event_aspect();

void send_before_event(const BeforeOperationEvent *event, char* buffer);
void send_after_event(const AfterOperationEvent *event, char* buffer);
void send_trace_meta_data(const TraceMetaData *meta, char* buffer);

#endif
