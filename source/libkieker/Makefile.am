SUBDIRS=common monitoring
ACLOCAL_AMFLAGS=-I m4 -g

lib_LTLIBRARIES = libkieker.la

libkieker_la_SOURCES = 
libkieker_la_LIBADD = common/record/controlflow/libcontrolflow.la \
	common/record/misc/libmisc.la \
	common/record/system/libsystem.la \
	common/record/database/libdatabase.la \
	common/record/jvm/libjvm.la \
	common/record/flow/trace/concurrency/libconcurrency.la \
	common/record/flow/trace/concurrency/monitor/libmonitor.la \
	common/record/flow/trace/operation/object/libobject.la \
	common/record/flow/trace/operation/constructor/libconstructor.la \
	common/record/flow/trace/operation/constructor/object/libobject.la \
	common/record/flow/trace/operation/liboperation.la \
	common/record/flow/trace/libtrace.la \
	common/record/flow/thread/libthread.la \
	common/record/remotecontrol/libremotecontrol.la \
	common/utilities/libutilities.la \
	monitoring/controller/libcontroller.la \
	monitoring/probes/libprobes.la \
	monitoring/probes/aspect/libaspect.la \
	monitoring/utilities/libutilities.la


