# use g++ for c files
set_source_files_properties(binary_serializer.c socket.c PROPERTIES LANGUAGE CXX)

ADD_LIBRARY(Util OBJECT
	binary_serializer.h
	binary_serializer.c
	socket.h
	socket.c
)
