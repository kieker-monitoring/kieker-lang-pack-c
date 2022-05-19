noinst_LTLIBRARIES = libcontroller.la

#######################################
# Build information for each library

# Sources for libkieker
libcontroller_la_SOURCES = kieker_io.c kieker_lookup.c kieker_controller.c kieker_adaptive_monitoring.c kieker_trace.c

# Linker options libTestProgram
libcontroller_la_LDFLAGS = -lbfd

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libcontroller_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

