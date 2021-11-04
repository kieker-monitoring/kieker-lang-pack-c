#ifndef OPERATION_EVENT_H
#define OPERATION_EVENT_H

typedef struct {
    long long timestamp;
    long long traceId;
    long int orderIndex;
    char *operationSignature;
    char *classSignature;
} OperationEvent;

typedef struct {
    long long traceId;
    long long threadId;
    char* sessionId;
    char* hostname;
    long long parentTraceId;
    long int parentOrderId;
} TraceMetaData;

int operation_event_serialize(char* buffer, const int id, const int offset, const OperationEvent* value);

int before_operation_event_serialize(char* buffer, const int offset, const OperationEvent* value);

int after_operation_event_serialize(char* buffer, const int offset, const OperationEvent* value);

int trace_meta_data_serialize(char* buffer, const int id, const int offset, const TraceMetaData* value);

#endif
