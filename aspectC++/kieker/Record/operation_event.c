#include "operation_event.h"
#include "Util/binary_serializer.h"

/*
 * serializes an event
 *
 * buffer = the buffer to write the serialization in
 * id = determine the type of event
 * offset = buffer offset
 * value = the data to serialize
 *
 * returns size of writen data
 */
int operation_event_serialize(char* buffer, const int id, const int offset, const OperationEvent* value) {
    int length = 0;
    length += serialize_int32(buffer,offset,id);
    length += serialize_int64(buffer,offset + length,value->timestamp);
    length += serialize_int64(buffer,offset + length,value->traceId);
    length += serialize_int32(buffer,offset + length,value->orderIndex);
    length += serialize_string(buffer,offset + length,value->operationSignature);
    length += serialize_string(buffer,offset + length,value->classSignature);

    return length;
}

/*
 * wraper function to serialize a before_operation_event
 *
 * returns size of writen data
 */
int before_operation_event_serialize(char* buffer, const int offset, const OperationEvent* value) {
    return operation_event_serialize(buffer, 20, offset, value);
}

/*
 * wraper function to serialize a after_operation_event
 *
 * returns size of writen data
 */
int after_operation_event_serialize(char* buffer, const int offset, const OperationEvent* value) {
    return operation_event_serialize(buffer, 30, offset, value);
}

/*
 * serialize a trace_metadata object
 *
 * buffer = the buffer to write the serialization in
 * id = determine type of object
 * offset = buffer offset
 * value = the data to serialize
 *
 * returns size of writen data
 */
int trace_meta_data_serialize(char* buffer, const int id, const int offset, const TraceMetaData* value) {
    int length = 0;
    length += serialize_int32(buffer,offset,id);
    length += serialize_int64(buffer,offset + length,value->traceId);
    length += serialize_int64(buffer,offset + length,value->threadId);
    length += serialize_string(buffer,offset + length,value->sessionId);
    length += serialize_string(buffer,offset + length,value->hostname);
    length += serialize_int64(buffer,offset + length,value->parentTraceId);
    length += serialize_int32(buffer,offset + length,value->parentOrderId);

    return length;
}
