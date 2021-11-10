noinst_LTLIBRARIES = libutilities.la

#######################################
# Build information for each library

# Sources for libkieker
libutilities_la_SOURCES = measure_time.c kieker_string.c

# Linker options libTestProgram
libutilities_la_LDFLAGS = -lbfd

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libutilities_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

