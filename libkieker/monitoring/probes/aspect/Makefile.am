noinst_LTLIBRARIES = libaspect.la

#######################################
# Build information for each library

# Sources for libkieker
libaspect_la_SOURCES = abstract_aspect.c event_aspect.c

# Linker options libTestProgram
libaspect_la_LDFLAGS = -lbfd

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libaspect_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

