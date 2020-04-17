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
#include "disk_usage_record.h"

/**
 * Author: Teerat Pitakrat
 * Version: 1.12
 */
/*
 * Serialize an DiskUsageRecord and return the size of the written structure.
 *
 * buffer = the buffer to send the data
 * id = id to identify the record type
 * offset = store data to buffer at offset
 * value = the value to be stored
 *
 * returns size of written structure
 */
int kieker_common_record_system_disk_usage_record_serialize(char *buffer, const int id, const int offset, const kieker_common_record_system_disk_usage_record value) {
	int length = 0;

	length += kieker_serialize_int64(buffer, offset, value.timestamp);
	length += kieker_serialize_string(buffer, offset, value.hostname);
	length += kieker_serialize_string(buffer, offset, value.deviceName);
	length += kieker_serialize_double(buffer, offset, value.queue);
	length += kieker_serialize_double(buffer, offset, value.readBytesPerSecond);
	length += kieker_serialize_double(buffer, offset, value.readsPerSecond);
	length += kieker_serialize_double(buffer, offset, value.serviceTime);
	length += kieker_serialize_double(buffer, offset, value.writeBytesPerSecond);
	length += kieker_serialize_double(buffer, offset, value.writesPerSecond);

	return length;
}
