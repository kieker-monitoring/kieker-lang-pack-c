#ifndef ABSTRACT_ASPECT_H
#define ABSTRACT_ASPECT_H

#include <common/record/controlflow/operation_execution_record.h>
#include "trace.h"

void init_aspect();
trace_hash_t* before_aspect(OperationExecutionRecord *record);
void after_aspect(trace_hash_t *data, OperationExecutionRecord* record);
void after_aspect2(trace_hash_t *data, OperationExecutionRecord* record);
void deinit_aspect();
void send_record(const OperationExecutionRecord* record, char* buffer);

#endif
