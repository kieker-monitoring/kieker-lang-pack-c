SUBDIRS=constructor object
ACLOCAL_AMFLAGS=-I m4
noinst_LTLIBRARIES = liboperation.la

#######################################
# Build information for each library

# Sources for libkieker
liboperation_la_SOURCES = after_operation_event.c after_operation_failed_event.c before_operation_event.c call_operation_event.c

# Linker options libTestProgram
liboperation_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
liboperation_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

