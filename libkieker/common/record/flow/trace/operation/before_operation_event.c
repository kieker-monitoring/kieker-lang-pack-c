/***************************************************************************
 * Copyright 2020 Kieker Project (http://kieker-monitoring.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***************************************************************************/
#include <stdlib.h>
#include <kieker.h>
#include "before_operation_event.h"

/**
 * Author: Jan Waller
 * Version: 1.5
 */
/*
 * Serialize an BeforeOperationEvent and return the size of the written structure.
 *
 * buffer = the buffer to send the data
 * id = id to identify the record type
 * offset = store data to buffer at offset
 * value = the value to be stored
 *
 * returns size of written structure
 */
int kieker_common_record_flow_trace_operation_before_operation_event_serialize(char *buffer, const int id, const int offset, const kieker_common_record_flow_trace_operation_before_operation_event value) {
	int position = offset;

	position += kieker_serialize_int64(buffer, position, value.timestamp);
	position += kieker_serialize_int64(buffer, position, value.traceId);
	position += kieker_serialize_int32(buffer, position, value.orderIndex);
	position += kieker_serialize_string(buffer, position, value.operationSignature);
	position += kieker_serialize_string(buffer, position, value.classSignature);

	return position;
}
