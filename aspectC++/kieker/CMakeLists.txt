#minimum version of cmake required to use this script
cmake_minimum_required(VERSION 2.4)

#set project name
PROJECT(Kieker)

#set project version number
SET(Kieker "0.1")

#set build type
#	None
#	Debug
#	Release
SET(CMAKE_BUILD_TYPE "Debug")

#verbose output
SET(CMAKE_VERBOSE_MAKEFILE "Off")

#set output directories. if not set, each binary will be placed where its source is
SET(LIBRARY_OUTPUT_PATH "${Kieker_BINARY_DIR}/lib")
SET(EXECUTABLE_OUTPUT_PATH "${Kieker_BINARY_DIR}/bin")

# build shared or static libraries (never can build both) ?
OPTION(BUILD_SHARED_LIBS "Build Kieker with shared libraries." ON)

#SET(USE_CXX_COMPILER true CACHE BOOL "Use CXX instead of C Compiler")

# use g++ for c files
#IF(${USE_CXX_COMPILER})
    #set_source_files_properties(${KIEKER_SOURCES} PROPERTIES LANGUAGE CXX)
#ENDIF()

#show all warings
#IF(${USE_CXX_COMPILER})
    #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -fPIC")
#ELSE()
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -W -Wall -fPIC")
#ENDIF()

SET(KIEKER_SOURCES
        controller.c
	abstract_aspect.c
        event_aspect.c

	Record/operation_execution_record.c
	Record/operation_event.c

	Util/binary_serializer.c
	Util/socket.c
	Util/measure_time.c
        Util/kieker_error.c

	Trace/trace.c

        Excluder/exclude_classes.c
)

SET(KIEKER_HEADER 
	kieker.h

        controller.h
	abstract_aspect.h
        event_aspect.h

	Record/operation_execution_record.h
	Record/operation_event.h

	Util/binary_serializer.h
	Util/socket.h
	Util/measure_time.h
	Util/uthash.h
        Util/kieker_error.h

	Trace/trace.h

        Excluder/exclude_classes.h
)

## base include directory 
INCLUDE_DIRECTORIES(
    ${Kieker_SOURCE_DIR}
)

ADD_LIBRARY(Kieker
	${KIEKER_SOURCES}
	${KIEKER_HEADER}
)

#add_subdirectory(Util)
#add_subdirectory(Record)

#ADD_LIBRARY(Kieker
#	abstract_aspect.h
#	abstract_aspect.c
#	$<TARGET_OBJECTS:Util> 
#	$<TARGET_OBJECTS:Record>
#)
#add_library(big ${other_srcs} $<TARGET_OBJECTS:A> $<TARGET_OBJECTS:B>)
#add_executable(testImage testImage.cpp)
#target_link_libraries(testImage Graphics2D)
#add_executable(convertPPMImage convertPPMImage.cpp)
#target_link_libraries(convertPPMImage Graphics2D)
