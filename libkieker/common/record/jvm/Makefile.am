noinst_LTLIBRARIES = libjvm.la

#######################################
# Build information for each library

# Sources for libkieker
libjvm_la_SOURCES = class_loading_record.c compilation_record.c g_c_record.c memory_record.c threads_status_record.c uptime_record.c

# Linker options libTestProgram
libjvm_la_LDFLAGS =

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libjvm_la_CPPFLAGS = -I$(top_srcdir)/libkieker/include

