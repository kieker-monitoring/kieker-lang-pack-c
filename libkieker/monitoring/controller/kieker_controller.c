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

enum kieker_init_states {
	UNCONFIGURED = 0, CONFIGURED = 1, FAILED = -1
};

int kieker_socket = 0;
int kieker_offset = 0;

int kieker_error = 0;
enum kieker_init_states kieker_init_state = UNCONFIGURED;

char *kieker_controller_buffer;
char *kieker_controller_string_buffer;
char *kieker_hostname;

pthread_rwlock_t kieker_controller_hash_lock;

/*
 * Internal controller functions.
 */
const char* kieker_controller_getenv_string(const char *name,
		const char *default_value);
unsigned short kieker_controller_getenv_ushort(const char *name,
		unsigned short default_value);

void kieker_controller_print_configuration(const char *hostname,
		unsigned short port);

/*
 * Initialize the kieker controller.
 */
void kieker_controller_initialize() {
	if (kieker_init_state == UNCONFIGURED) {
		int ret = pthread_rwlock_init(&kieker_controller_hash_lock, NULL);
		if (ret != 0) {
			KIEKER_ERROR("Could not create rwlock!");
			kieker_init_state = FAILED;
			return;
		}
		const char *host = kieker_controller_getenv_string("KIEKER_HOSTNAME",
				"localhost");
		unsigned short port = kieker_controller_getenv_ushort("KIEKER_PORT",
				5678);
		const char *register_event_types_filename =
				kieker_controller_getenv_string("KIEKER_EVENT_TYPE_FILE",
						"~/kieker-event-type-file");

		kieker_offset = 0;
		kieker_controller_buffer = malloc(8192 * sizeof(char));
		kieker_controller_string_buffer = malloc(8192 * sizeof(char));

		kieker_trace_init_id();

		if ((kieker_socket = kieker_io_client_socket_open(host, port)) != -1) {
			kieker_controller_register_event_types(
					register_event_types_filename);

			kieker_hostname = malloc(200);
			if (gethostname(kieker_hostname, 199) == -1) {
				kieker_error = errno;
				fprintf(stderr, "Failure looking up hostname: %s\n",
						strerror(kieker_error));
				kieker_hostname = "<failed>";
			}
			kieker_controller_print_configuration(host, port,
					register_event_types_filename);
			int error_code = atexit(kieker_controller_finalize);
			if (error_code != 0) {
				fprintf(stderr, "Cannot set exit function.\n");
				kieker_init_state = FAILED;
				kieker_controller_finalize();
			}
		} else {
			fprintf(stderr, "Cannot setup connection.\n");
			kieker_init_state = FAILED;
		}
	}
}

/*
 * Read a file in to configure register all event types we intend to use in the analysis.
 */
void kieker_controller_register_event_types(const char *filename) {
	if (access(filename, F_OK) == 0) {
		FILE fin;
		const char read_buffer[1024];
		if ((fin = fopen(filename, "r"))) {
			while (getline(read_buffer, 1023, fin) != -1) {
				char ch = read_buffer[strlen(read_buffer) - 1];
				while (ch == '\r' || ch == '\n') {
					read_buffer[strlen(read_buffer) - 1] = 0;
					ch = read_buffer[strlen(read_buffer) - 1];
				}
				// TODO use trim here
				kieker_offset += kieker_serialize_string(
						kieker_controller_buffer, kieker_offset, read_buffer);
			}
		}
	} else {
		// assume minimal trace setup.
		kieker_offset += kieker_serialize_string(kieker_controller_buffer,
				kieker_offset, "kieker.common.record.flow.trace.TraceMetadata");
		kieker_offset +=
				kieker_serialize_string(kieker_controller_buffer, kieker_offset,
						"kieker.common.record.flow.trace.operation.BeforeOperationEvent");
		kieker_offset +=
				kieker_serialize_string(kieker_controller_buffer, kieker_offset,
						"kieker.common.record.flow.trace.operation.AfterOperationEvent");
	}
	kieker_offset = 0;
}

/*
 * Global buffer for kieker IO.
 */
char *kieker_controller_buffer = NULL;

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
	kieker_serialize_int32(kieker_controller_string_buffer, 0, -1);
	kieker_serialize_int32(kieker_controller_string_buffer, 4, id);
	kieker_serialize_int32(kieker_controller_string_buffer, 8, length);
	strncpy(kieker_controller_string_buffer + 4 + 4 + 4, string, length);

	if (write(kieker_socket, kieker_controller_string_buffer,
			length + 4 + 4 + 4) == -1) {
		kieker_error = errno;
		fprintf(stderr, "Write failure sending initial strings: %s\n",
				strerror(kieker_error));
		kieker_init_state = FAILED;
	}
	fsync(kieker_socket);
}

/**
 * Send all data in the buffer to the stream.
 *
 * length = length of the data stored in the buffer
 */
void kieker_controller_send(int length) {
	if (write(kieker_socket, kieker_controller_buffer, length) == -1) {
		kieker_error = errno;
		fprintf(stderr, "Write failure: %s\n", strerror(kieker_error));
	}
	fsync(kieker_socket);
}

/*
 * Return the buffer.
 */
char* kieker_controller_get_buffer() {
	return kieker_controller_buffer;
}

/*
 * Return the hostname.
 */
const char* kieker_controller_get_hostname() {
	return kieker_hostname;
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
int kieker_monitoring_controller_prefix_serialize(int id, int offset) {
	int position = offset;

	position += kieker_serialize_int32(kieker_controller_buffer, position, id);
	position += kieker_serialize_int64(kieker_controller_buffer, position,
			kieker_controller_get_time_ms());

	return position;
}

void kieker_controller_print_configuration(const char *hostname,
		unsigned short port, const char *event_type_filename) {
	fprintf(stdout, "Kieker Configuration\n");
	fprintf(stdout, "\tcollector hostname = %s\n", hostname);
	fprintf(stdout, "\tcollector port = %d\n", port);
	fprintf(stdout, "\tevent type registrytion file = %s\n", event_type_filename);
}

void kieker_controller_finalize() {
	kieker_io_socket_close(kieker_socket);
	pthread_rwlock_destroy(&kieker_controller_hash_lock);
	kieker_init_state = UNCONFIGURED;
}
