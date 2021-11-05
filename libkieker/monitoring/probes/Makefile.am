SUBDIRS=aspect
ACLOCAL_AMFLAGS=-I m4
noinst_LTLIBRARIES = libprobes.la

#######################################
# Build information for each library

# Sources for libkieker
libprobes_la_SOURCES = kieker_probe_gcc_profiling.c kieker_probe_plain.c

# Linker options libTestProgram
libprobes_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libprobes_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

