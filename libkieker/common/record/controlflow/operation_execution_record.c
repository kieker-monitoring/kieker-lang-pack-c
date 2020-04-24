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
#include "operation_execution_record.h"

/**
 * Author: Andre van Hoorn, Jan Waller
 * Version: 0.91
 */
/*
 * Serialize an OperationExecutionRecord and return the size of the written structure.
 *
 * buffer = the buffer to send the data
 * offset = store data to buffer at offset
 * value = the value to be stored
 *
 * returns size of written structure
 */
int kieker_common_record_controlflow_operation_execution_record_serialize(char *buffer, const int offset, const kieker_common_record_controlflow_operation_execution_record value) {
	int position = offset;

	position += kieker_serialize_string(buffer, position, value.operationSignature);
	position += kieker_serialize_string(buffer, position, value.sessionId);
	position += kieker_serialize_int64(buffer, position, value.traceId);
	position += kieker_serialize_int64(buffer, position, value.tin);
	position += kieker_serialize_int64(buffer, position, value.tout);
	position += kieker_serialize_string(buffer, position, value.hostname);
	position += kieker_serialize_int32(buffer, position, value.eoi);
	position += kieker_serialize_int32(buffer, position, value.ess);

	return position;
}
