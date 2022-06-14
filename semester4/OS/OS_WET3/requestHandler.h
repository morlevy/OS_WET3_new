#ifndef OS_WET3_REQUEST_HANDLER
#define OS_WET3_REQUEST_HANDLER
#include <pthread.h>
#include "requestQueue.h"

typedef struct request_data{
    int fd;
    struct timeval* entry_time;
}*requestData;

requestData createRequestData(int fd, struct timeval* entry_time);

typedef struct request_handler{
    Queue waiting;
    Queue working;
    int max_capacity;
    int current_size;
    pthread_cond_t enq;
    pthread_cond_t deq;
    pthread_mutex_t mutex;
}*requestHandler;

requestHandler createHandler(int max_capacity);
void addRequest(requestHandler handler , requestData data);
void deleteRequest(requestHandler handler , Node node);
Node executeRequest(requestHandler handler);
int getCurrentSize(requestHandler);
void decreaseCurrentSize(requestHandler handler);

#endif
