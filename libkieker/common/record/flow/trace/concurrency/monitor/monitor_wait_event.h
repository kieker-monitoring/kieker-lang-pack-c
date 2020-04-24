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

/*
 * Author: Jan Waller
 * Version: 1.8
 */
typedef struct {
	long long timestamp;
	long long traceId;
	long orderIndex;
	long lockId;
} kieker_common_record_flow_trace_concurrency_monitor_monitor_wait_event;

/*
 * Serialize an MonitorWaitEvent and return the size of the written structure.
 *
 * buffer = the buffer to send the data
 * offset = store data to buffer at offset
 * value = the value to be stored
 *
 * returns size of written structure
 */
int kieker_common_record_flow_trace_concurrency_monitor_monitor_wait_event_serialize(char *buffer, const int offset, const kieker_common_record_flow_trace_concurrency_monitor_monitor_wait_event value);
