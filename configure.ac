dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT(kieker-lang-pack-c, 1.0)
AC_PROG_CC

AC_CONFIG_MACRO_DIRS([m4])

AC_CANONICAL_SYSTEM

dnl Initialize automake
AM_INIT_AUTOMAKE([foreign])

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries
AC_PROG_LIBTOOL

AC_CONFIG_FILES(Makefile
examples/Makefile
examples/basic-example/Makefile
examples/instrument-example/Makefile
examples/bfd/Makefile
libkieker/common/record/controlflow/Makefile
libkieker/common/record/misc/Makefile
libkieker/common/record/system/Makefile
libkieker/common/record/database/Makefile
libkieker/common/record/jvm/Makefile
libkieker/common/record/flow/trace/Makefile
libkieker/common/record/flow/trace/concurrency/Makefile
libkieker/common/record/flow/trace/concurrency/monitor/Makefile
libkieker/common/record/flow/trace/operation/object/Makefile
libkieker/common/record/flow/trace/operation/Makefile
libkieker/common/record/flow/trace/operation/constructor/object/Makefile
libkieker/common/record/flow/trace/operation/constructor/Makefile
libkieker/common/record/flow/thread/Makefile
libkieker/common/record/flow/Makefile
libkieker/common/record/Makefile
libkieker/common/record/remotecontrol/Makefile
libkieker/common/utilities/Makefile
libkieker/common/Makefile
libkieker/monitoring/probes/Makefile
libkieker/monitoring/Makefile
libkieker/monitoring/controller/Makefile
libkieker/Makefile
libkieker/include/Makefile)

AC_OUTPUT
