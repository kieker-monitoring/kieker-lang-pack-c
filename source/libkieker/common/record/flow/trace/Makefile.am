SUBDIRS=concurrency operation
ACLOCAL_AMFLAGS=-I m4
noinst_LTLIBRARIES = libtrace.la

#######################################
# Build information for each library

# Sources for libkieker
libtrace_la_SOURCES = application_trace_metadata.c before_received_remote_event.c before_sent_remote_event.c construction_event.c trace_metadata.c

# Linker options libTestProgram
libtrace_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libtrace_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

