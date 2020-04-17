noinst_LTLIBRARIES = libmonitor.la

#######################################
# Build information for each library

# Sources for libkieker
libmonitor_la_SOURCES = monitor_entry_event.c monitor_exit_event.c monitor_notify_all_event.c monitor_notify_event.c monitor_request_event.c monitor_wait_event.c

# Linker options libTestProgram
libmonitor_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmonitor_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

