dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([kieker-lang-pack-c],[1.0])
AC_CONFIG_AUX_DIR(autotools)
AC_CONFIG_MACRO_DIR(m4)

AM_PROG_AR
 
: ${CFLAGS="-O3 -Wall -Wextra -Wpedantic -std=c11"}
AC_PROG_CC

AC_CONFIG_MACRO_DIRS([m4])

AC_CHECK_LIB(pthread, pthread_rwlock_init)
AC_CHECK_FUNCS([pthread_self pthread_rwlock_rdlock pthread_rwlock_unlock pthread_rwlock_wrlock pthread_rwlock_destroy])

AC_CHECK_FUNCS([bind close listen connect setsockopt socket write])

AC_CHECK_FUNCS([fclose fgets fopen calloc fsync fwrite getline])

AC_CHECK_FUNCS([malloc free realloc memcpy])

AC_CHECK_FUNCS([exit getenv gethostbyname gethostname qsort rand srand])

AC_CHECK_FUNCS([time gettimeofday])

AC_CHECK_FUNCS([strcmp strerror strlen strncpy strtol])



dnl Initialize automake
AM_INIT_AUTOMAKE([1.16 subdir-objects -Wall -Werror no-define])

dnl this allows us specify individual liking flags for each target
AM_PROG_CC_C_O

dnl Initialize Libtool
LT_INIT

dnl Check if Libtool is present
dnl Libtool is used for building share libraries
LT_INIT

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
libkieker/monitoring/Makefile
libkieker/monitoring/controller/Makefile
libkieker/monitoring/probes/Makefile
libkieker/monitoring/probes/aspect/Makefile
libkieker/monitoring/utilities/Makefile
libkieker/Makefile
libkieker/include/Makefile
tests/Makefile)

AC_OUTPUT
