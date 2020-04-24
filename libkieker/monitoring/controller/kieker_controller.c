/*
 * kieker_controller.c
 *
 *  Created on: Apr 17, 2020
 *      Author: reiner
 */

#include "kieker_controller.h"
#include "kieker_io.h"
#include "kieker_serialize.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#define _DEBUG DEBUG

enum kieker_init_states {
	UNCONFIGURED = 0, CONFIGURED = 1, FAILED = -1
};

long kieker_thread_array_size = 0;
kieker_thread_array_entry *kieker_thread_array = NULL;

int kieker_socket = 0;
int kieker_offset = 0;

int kieker_error = 0;
enum kieker_init_states kieker_init_state = UNCONFIGURED;

char *kieker_controller_string_buffer;
char *kieker_hostname;

// temporary test
int kwrite(int fd, const void *buf, size_t count, const char* caller);

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

/*
 * Find a thread entry and return it. In case no thread entry is present return NULL.
 * If no thread is currently in the list, return NULL
 */
kieker_thread_array_entry* kieker_controller_create_thread_entry() {
	if (kieker_thread_array_size == 0) {
		kieker_thread_array = malloc(sizeof(kieker_thread_array_entry));
	} else {
		kieker_thread_array = realloc(kieker_thread_array,
				sizeof(kieker_thread_array_entry)
						* (kieker_thread_array_size + 1));
	}

	kieker_thread_array[kieker_thread_array_size].process_id = getpid();
	kieker_thread_array[kieker_thread_array_size].thread_id = getpid(); // TODO gettid seems not to be available and is also not POSIX conform
	kieker_thread_array[kieker_thread_array_size].trace_id = 0;

	kieker_thread_array_size++;

	return &kieker_thread_array[kieker_thread_array_size - 1];
}

/*
 * Initialize the kieker controller.
 */
void kieker_controller_initialize() {
	// TODO this must be read from configuration
	char *host = "localhost";
	unsigned short port = 5678;
	kieker_offset = 0;
	kieker_controller_buffer = malloc(8192 * sizeof(char));
	kieker_controller_string_buffer = malloc(8192 * sizeof(char));

	if ((kieker_socket = kieker_io_client_socket_open(host, port)) != -1) {
		// TODO this must loop over a record registration file
		kieker_offset += kieker_serialize_string(kieker_controller_buffer,
				kieker_offset, "kieker.common.record.flow.trace.TraceMetadata");
		kieker_offset +=
				kieker_serialize_string(kieker_controller_buffer, kieker_offset,
						"kieker.common.record.flow.trace.operation.BeforeOperationEvent");
		kieker_offset +=
				kieker_serialize_string(kieker_controller_buffer, kieker_offset,
						"kieker.common.record.flow.trace.operation.AfterOperationEvent");

		kieker_offset = 0;

		kieker_hostname = malloc(200);
		if (gethostname(kieker_hostname, 199) == -1) {
			kieker_error = errno;
			fprintf(stderr, "Failure looking up hostname: %s\n",
					strerror(kieker_error));
			kieker_hostname = "<failed>";
		}
	} else {
		fprintf(stderr, "Cannot setup connection.\n");
		kieker_init_state = FAILED;
	}
}

// TODO register shutdown hook

/**
 * Append a string to the send buffer.
 */
void kieker_controller_send_string(const char *string, int id) {
	int length = strlen(string);
	kieker_serialize_int32(kieker_controller_string_buffer, 0, -1);
	kieker_serialize_int32(kieker_controller_string_buffer, 4, id);
	kieker_serialize_int32(kieker_controller_string_buffer, 8, length);
	strncpy(kieker_controller_string_buffer + 4 + 4 + 4, string, length);

	if (kwrite(kieker_socket, kieker_controller_string_buffer, length + 4 + 4 + 4, "send string")
			== -1) {
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
	if (kwrite(kieker_socket, kieker_controller_buffer, length, "record send") == -1) {
		kieker_error = errno;
		fprintf(stderr, "Write failure: %s\n", strerror(kieker_error));
	}
	fsync(kieker_socket);
}

/*
 * Find a thread entry and return it. In case no thread entry is present return NULL.
 * If no thread is currently in the list, return NULL
 */
kieker_thread_array_entry* kieker_controller_get_thread_entry() {
	if (kieker_thread_array_size != 0) {
		pid_t current_thread_id = getpid(); // TODO gettid might not work we need a posix conform alternative
		for (int i = 0; i < kieker_thread_array_size; i++) {
			if (kieker_thread_array[i].thread_id == current_thread_id) {
				return &kieker_thread_array[i];
			}
		}

		return kieker_controller_create_thread_entry();
	} else {
		kieker_controller_initialize();
		return kieker_controller_create_thread_entry();
	}
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

/**
 * Lookup the function name and parameters and produce a FQN string from it.
 *
 * Return the complete string or "<lookup failed %p>"
 */
const char* kieker_controller_get_operation_fqn(void *function_ptr) {
	char *result = malloc(16 + 2 + 16 + 1);

	sprintf(result, "<lookup failed %p>", function_ptr);

	return result;
}

/**
 * Write the id and logging timestamp to the buffer.
 *
 * id = id of the record type.
 * offset = offset in the buffer
 */
int kieker_monitoring_controller_prefix_serialize(int id, int offset) {
	fprintf(stderr,"ID %d\n", id);
	int position = offset;

	position += kieker_serialize_int32(kieker_controller_buffer, position, id);
	position += kieker_serialize_int64(kieker_controller_buffer, position, kieker_controller_get_time_ms());

	return position;
}

#define WIDTH 16
#define REST (WIDTH - 1)

int kwrite(int fd, const void *buf, size_t count, const char *caller) {
#ifdef _DEBUG
	fprintf(stderr, "Write: %s ------------------\n", caller);
	int i;
	const char* read_buf = (const char*) buf;
	for (i = 0; i < count; i++) {
		char ch = *read_buf;
		read_buf++;
		fprintf(stderr, "%02x ", (unsigned char)ch);
		if (i % (WIDTH/2) == (WIDTH/2-1)) {
			fprintf(stderr," ");
		}
		if (i % WIDTH == REST) {
			int j;
			const char *read_buf2 = read_buf - REST;
			for (j = 0; j < WIDTH; j++) {
				char ch2 = *read_buf2;
				read_buf2++;
				if (ch2 > 31) {
					fprintf(stderr, "%c", ch2);
				} else {
					fprintf(stderr, ".");
				}
			}
			fprintf(stderr, "\n");
		}
	}
	if (i % WIDTH != 0) {
		int j;
		for (j = 0; j < WIDTH - i % WIDTH; j++) {
			fprintf(stderr, "   ");
		}
		const char *read_buf2 = read_buf - i % WIDTH;
		for (j = 0; j < i % WIDTH; j++) {
			char ch2 = *read_buf2;
			read_buf2++;
			if (ch2 > 31) {
				fprintf(stderr, "%c", ch2);
			} else {
				fprintf(stderr, ".");
			}
		}
		fprintf(stderr, "\n");
	}

	fprintf(stderr, "\n-------------------------\n");
#endif
	return write(fd, buf, count);
}

