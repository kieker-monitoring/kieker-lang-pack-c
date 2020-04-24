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
#include "call_operation_object_event.h"

/**
 * Author: Jan Waller
 * Version: 1.6
 */
/*
 * Serialize an CallOperationObjectEvent and return the size of the written structure.
 *
 * buffer = the buffer to send the data
 * offset = store data to buffer at offset
 * value = the value to be stored
 *
 * returns size of written structure
 */
int kieker_common_record_flow_trace_operation_object_call_operation_object_event_serialize(char *buffer, const int offset, const kieker_common_record_flow_trace_operation_object_call_operation_object_event value) {
	int position = offset;

	position += kieker_serialize_int64(buffer, position, value.timestamp);
	position += kieker_serialize_int64(buffer, position, value.traceId);
	position += kieker_serialize_int32(buffer, position, value.orderIndex);
	position += kieker_serialize_string(buffer, position, value.operationSignature);
	position += kieker_serialize_string(buffer, position, value.classSignature);
	position += kieker_serialize_string(buffer, position, value.calleeOperationSignature);
	position += kieker_serialize_string(buffer, position, value.calleeClassSignature);
	position += kieker_serialize_int32(buffer, position, value.objectId);
	position += kieker_serialize_int32(buffer, position, value.calleeObjectId);

	return position;
}
