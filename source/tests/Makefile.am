ACLOCAL_AMFLAGS = -I m4

check_PROGRAMS = kieker_trace_test kieker_controller_test
TESTS = kieker_trace_test kieker_controller_test

kieker_trace_test_CFLAGS =  -I$(top_srcdir)/libkieker/monitoring/controller
kieker_trace_test_LDFLAGS = -lcmocka

kieker_controller_test_CFLAGS =  -I$(top_srcdir)/libkieker/monitoring/controller -I$(top_srcdir)/libkieker/include
kieker_controller_test_LDFLAGS = -lcmocka

LDADD = $(top_srcdir)/libkieker/.libs/libkieker.la
