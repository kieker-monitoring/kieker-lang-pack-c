#ifndef KIEKER_PROBES_ASPECT_CONTROLFLOW_H
#define KIEKER_PROBES_ASPECT_CONTROLFLOW_H

#include "../../../common/record/controlflow/operation_execution_record.h"
#include "../../controller/kieker_trace.h"

void kieker_probe_aspect_controlflow_initialize();
kieker_trace_t*  kieker_probe_aspect_controlflow_before(kieker_common_record_controlflow_operation_execution_record *record);
void kieker_probe_aspect_controlflow_after(kieker_trace_t *data, kieker_common_record_controlflow_operation_execution_record* record);
void kieker_probe_aspect_controlflow_after2(kieker_trace_t *data, kieker_common_record_controlflow_operation_execution_record* record);
void kieker_probe_aspect_controlflow_finalize();
void kieker_probe_aspect_controlflow_send(const kieker_common_record_controlflow_operation_execution_record* record, char* buffer);

#endif
