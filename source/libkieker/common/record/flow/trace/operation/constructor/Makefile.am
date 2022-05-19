SUBDIRS=object
ACLOCAL_AMFLAGS=-I m4
noinst_LTLIBRARIES = libconstructor.la

#######################################
# Build information for each library

# Sources for libkieker
libconstructor_la_SOURCES = after_constructor_event.c after_constructor_failed_event.c before_constructor_event.c call_constructor_event.c

# Linker options libTestProgram
libconstructor_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libconstructor_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

