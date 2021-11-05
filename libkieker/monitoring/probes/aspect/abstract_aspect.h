#ifndef ABSTRACT_ASPECT_H
#define ABSTRACT_ASPECT_H

#include "../../../common/record/controlflow/operation_execution_record.h"
#include "../../controller/kieker_trace.h"

void init_aspect();
kieker_trace_hash_t* before_aspect(kieker_common_record_controlflow_operation_execution_record *record);
void after_aspect(kieker_trace_hash_t *data, kieker_common_record_controlflow_operation_execution_record* record);
void after_aspect2(kieker_trace_hash_t *data, kieker_common_record_controlflow_operation_execution_record* record);
void deinit_aspect();
void send_record(const kieker_common_record_controlflow_operation_execution_record* record, char* buffer);

#endif
