bin_PROGRAMS = instrument
instrument_SOURCES = instrument.c

LDADD = ../../libkieker/.libs/libkieker.la  -ldl

# instrument_CFLAGS = $(AM_FLAGS) -I../../libkieker/common/record/controlflow -I../../libkieker/include -finstrument-functions -g
instrument_CFLAGS = $(AM_FLAGS) -I../../libkieker/common/record/controlflow -I../../libkieker/include -g

