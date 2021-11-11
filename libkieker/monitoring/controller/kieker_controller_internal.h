#ifndef _KIEKER_CONTROLLER_INTERNAL_H
#define _KIEKER_CONTROLLER_INTERNAL_H

#define KIEKER_DEFAULT_REMOTE_HOSTNAME "localhost"
#define KIEKER_DEFAULT_REMOTE_PORT 5678
#define KIEKER_DEFAULT_EVENT_TYPE_FILENAME "~/kieker-event-type-file"

enum kieker_init_states {
	UNCONFIGURED = 0, CONFIGURED = 1, FAILED = -1
};

typedef struct kieker_controller_state {
	int socket;
	int offset;
	int error;
	enum kieker_init_states init_state;
	char *buffer;
	char *string_buffer;
	char *local_hostname;
	const char *remote_hostname;
	unsigned short remote_port;
	const char *event_types_filename;
} kieker_controller_state_t;

/**
 * Read environment variable and return its value as a string. In case the variable is
 * not set return the default value.
 */
const char* kieker_controller_getenv_string(const char *name,
		const char *default_value);
/**
 * Read environment variable and return its value as a unsigned short. In case the variable is
 * not set return the default value.
 */
unsigned short kieker_controller_getenv_ushort(const char *name,
		unsigned short default_value);

void kieker_controller_configure_variables();
void kieker_controller_setup_buffer();
int kieker_controller_obtain_local_hostname();
void kieker_controller_connect();

/*
 * Read a file in to configure register all event types we intend to use in the analysis.
 */
void kieker_controller_register_event_types(const char *filename);

/**
 * Print out the configuration used for Kieker.
 */
void kieker_controller_print_configuration();

#endif
