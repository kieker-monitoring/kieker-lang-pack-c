# use g++ for c files
set_source_files_properties(operation_execution_record.c PROPERTIES LANGUAGE CXX)

ADD_LIBRARY(Record OBJECT
	operation_execution_record.h
	operation_execution_record.c
)
