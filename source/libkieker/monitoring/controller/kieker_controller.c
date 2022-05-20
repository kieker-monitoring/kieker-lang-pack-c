/*
 * kieker_controller.c
 *
 *  Created on: Apr 17, 2020
 *      Author: Reiner Jung
 */

#include "kieker_controller.h"
#include "kieker_io.h"
#include "kieker_serialize.h"
#include "kieker_trace.h"
#include "../utilities/kieker_error.h"
#include "../utilities/kieker_string.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define _GNU_SOURCE
#define __USE_GNU

#include <dlfcn.h>
#include <execinfo.h>
#include <link.h>
#include <limits.h>
#include "kieker_controller_internal.h"

kieker_controller_state_t kieker_controller = { 0, 0, 0, UNCONFIGURED, NULL, NULL, NULL, NULL, 0, };

__attribute__((constructor)) void init(void) {
	kieker_controller_initialize();
}
__attribute__((destructor))  void fini(void) {
	kieker_controller_finalize();
}

/*
 * Initialize the kieker controller.
 */
void kieker_controller_initialize() {
	if (kieker_controller.init_state == UNCONFIGURED) {
		kieker_controller_configure_variables();
		kieker_controller_setup_buffer();
		if (kieker_controller_obtain_local_hostname()) {
			if (kieker_trace_init()) {
				kieker_controller_connect();
				kieker_controller_register_event_types();
			} else {
				kieker_controller.init_state = FAILED;
			}
		} else {
			kieker_controller.init_state = FAILED;
		}
	}
}

void kieker_controller_connect() {
	if ((kieker_controller.socket = kieker_io_client_socket_open(
			kieker_controller.remote_hostname, kieker_controller.remote_port))
			!= -1) {
		kieker_controller_print_configuration();
		int error_code = atexit(kieker_controller_finalize);
		if (error_code != 0) {
			KIEKER_ERROR_PLAIN("Cannot set exit function.");
			kieker_controller_finalize();
			kieker_controller.init_state = FAILED;
		}
	} else {
		KIEKER_ERROR_PLAIN("Cannot setup connection.");
		kieker_controller.init_state = FAILED;
	}
}

/*
 * Read in environment variables.
 */
void kieker_controller_configure_variables() {
	kieker_controller.remote_hostname = kieker_controller_getenv_string(
			"KIEKER_HOSTNAME", KIEKER_DEFAULT_REMOTE_HOSTNAME);
	kieker_controller.remote_port = kieker_controller_getenv_ushort(
			"KIEKER_PORT", KIEKER_DEFAULT_REMOTE_PORT);
	kieker_controller.event_types_filename = kieker_controller_getenv_string(
			"KIEKER_EVENT_TYPE_FILENAME", KIEKER_DEFAULT_EVENT_TYPE_FILENAME);
}

void kieker_controller_setup_buffer() {
	kieker_controller.offset = 0;
	kieker_controller.buffer = malloc(8192 * sizeof(char));
	kieker_controller.string_buffer = malloc(8192 * sizeof(char));
}

int kieker_controller_obtain_local_hostname() {
	kieker_controller.local_hostname = malloc(200);
	if (gethostname(kieker_controller.local_hostname, 199) == -1) {
		kieker_controller.error = errno;
		KIEKER_ERROR("Failure looking up hostname: %s\n",
				strerror(kieker_controller.error));
		kieker_controller.local_hostname = "<failed>";
		return 0;
	} else
		return 1;
}

/*
 * Register basic record types.
 */
void kieker_controller_register_event_types() {
	// assume minimal trace setup.
	kieker_controller.offset += kieker_serialize_string(
			kieker_controller.buffer, kieker_controller.offset,
			"kieker.common.record.flow.trace.TraceMetadata");
	kieker_controller.offset += kieker_serialize_string(
			kieker_controller.buffer, kieker_controller.offset,
			"kieker.common.record.flow.trace.operation.BeforeOperationEvent");
	kieker_controller.offset += kieker_serialize_string(
			kieker_controller.buffer, kieker_controller.offset,
			"kieker.common.record.flow.trace.operation.AfterOperationEvent");

	kieker_controller.offset = 0;
}

/*
 * Return the current time since epoch in milliseconds.
 */
long long kieker_controller_get_time_ms() {
	struct timeval call_time;
	gettimeofday(&call_time, NULL);
	return ((long long) call_time.tv_sec * 1000000) + call_time.tv_usec;
}

const char* kieker_controller_getenv_string(const char *name,
		const char *default_value) {
	const char *value = getenv(name);
	if (value == NULL) {
		return default_value;
	} else {
		return value;
	}
}

unsigned short kieker_controller_getenv_ushort(const char *name,
		unsigned short default_value) {
	const char *value = getenv(name);
	if (value == NULL) {
		return default_value;
	} else {
		char *end;
		long result = strtol(value, &end, 10);
		if (value == end) {
			fprintf(stderr, "Port number %s cannot be converted to a number.\n",
					value);
			return default_value;
		} else {
			if (result > 0 && result <= USHRT_MAX) {
				return (unsigned short) result;
			} else {
				fprintf(stderr, "Port value %ld is out of bounds [%d:%d].\n",
						result, 1, USHRT_MAX);
				return default_value;
			}
		}
	}
}

/**
 * Append a string to the send buffer.
 */
void kieker_controller_send_string(const char *string, int id) {
	int length = strlen(string);
	kieker_serialize_int32(kieker_controller.string_buffer, 0, -1);
	kieker_serialize_int32(kieker_controller.string_buffer, 4, id);
	kieker_serialize_int32(kieker_controller.string_buffer, 8, length);
	strncpy(kieker_controller.string_buffer + 4 + 4 + 4, string, length);

	if (write(kieker_controller.socket, kieker_controller.string_buffer,
			length + 4 + 4 + 4) == -1) {
		kieker_controller.error = errno;
		fprintf(stderr, "Write failure sending initial strings: %s\n",
				strerror(kieker_controller.error));
		kieker_controller.init_state = FAILED;
	}
	fsync(kieker_controller.socket);
}

/**
 * Send all data in the buffer to the stream.
 *
 * length = length of the data stored in the buffer
 */
void kieker_controller_send(int length) {
	if (write(kieker_controller.socket, kieker_controller.buffer, length)
			== -1) {
		kieker_controller.error = errno;
		fprintf(stderr, "Write failure: %s\n",
				strerror(kieker_controller.error));
	}
	fsync(kieker_controller.socket);
}

/*
 * Return the buffer.
 */
char* kieker_controller_get_buffer() {
	return kieker_controller.buffer;
}

/*
 * Return the hostname.
 */
const char* kieker_controller_get_hostname() {
	return kieker_controller.local_hostname;
}

void* kieker_controller_convert_to__vma(void *addr) {
	Dl_info info;
	struct link_map *link_map;
	dladdr1(addr, &info, (void**) &link_map, RTLD_DL_LINKMAP);

	return addr - link_map->l_addr;
}

/** Constant conversion buffer to store %p output. 2 for the 0x prefix, 16 for hex-digits, 1 for 0 termination. */
char kieker_controllor_get_operation_fqn_result[2 + 16 + 1];
/**
 * Lookup the function name and parameters and produce a FQN string from it.
 *
 * Return the complete string or "<lookup failed %p>"
 */
const char* kieker_controller_get_operation_fqn(void *function_ptr) {
	sprintf(kieker_controllor_get_operation_fqn_result, "%p",
			kieker_controller_convert_to__vma(function_ptr));

//	sprintf(result, "%p", function_ptr);

	return kieker_controllor_get_operation_fqn_result;
}

/**
 * Write the id and logging timestamp to the buffer.
 *
 * id = id of the record type.
 * offset = offset in the buffer
 */
int kieker_controller_prefix_serialize(int id, int offset) {
	int position = offset;

	position += kieker_serialize_int32(kieker_controller.buffer, position, id);
	position += kieker_serialize_int64(kieker_controller.buffer, position,
			kieker_controller_get_time_ms());

	return position;
}

void kieker_controller_print_configuration() {
	fprintf(stdout, "Kieker Configuration\n");
	fprintf(stdout, "\tcollector hostname = %s\n",
			kieker_controller.remote_hostname);
	fprintf(stdout, "\tcollector port = %d\n", kieker_controller.remote_port);
}

void kieker_controller_finalize() {
	kieker_io_socket_close(kieker_controller.socket);
	kieker_trace_finalize();
	kieker_controller.init_state = UNCONFIGURED;
}
