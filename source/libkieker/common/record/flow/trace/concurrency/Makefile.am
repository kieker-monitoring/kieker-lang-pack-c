SUBDIRS=monitor
ACLOCAL_AMFLAGS=-I m4
noinst_LTLIBRARIES = libconcurrency.la

#######################################
# Build information for each library

# Sources for libkieker
libconcurrency_la_SOURCES = join_event.c split_event.c

# Linker options libTestProgram
libconcurrency_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libconcurrency_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

