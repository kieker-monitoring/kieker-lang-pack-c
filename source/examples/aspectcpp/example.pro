ASPECT_HEADERS += event_trace_total.ah
#ASPECT_HEADERS += event_trace_withoutMain.ah
load(/home/soeren/programs/AOP/acxx.prf)

#CONFIG += qt acxx
CONFIG += acxx

HEADERS += math/fibs.hh \
            math/power.h \
            string/string_test.h \
            measurement/measurement.h
SOURCES += main.cpp \
            math/fibs.cpp \
            math/power.c \
            string/string_test.c \
            measurement/measurement.c

QMAKE_CXXFLAGS += -Wall
QMAKE_LFLAGS += -fopenmp

TARGET = fibs

INCLUDEPATH += /home/soeren/programs/aop-instrumentation-for-c-programs-with-kieker/Code/kieker/
LIBS += -L/home/soeren/programs/AOP/kieker-build/lib/ -lKieker
