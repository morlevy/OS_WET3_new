#include "requestHandler.h"
#include <stdlib.h>

requestHandler createHandler(int max_capacity){
    requestHandler handler = (requestHandler) malloc(sizeof(*handler));
    handler->max_capacity = max_capacity;
    handler->waiting = createQueue();
    handler->working = createQueue();
    handler->current_size = 0;
    pthread_mutex_init(&handler->mutex,NULL);
    pthread_cond_init(&handler->enq,NULL);
    pthread_cond_init(&handler->deq,NULL);
    return handler;
}

requestData createRequestData(int fd, struct timeval* entry_time){
    requestData data = (requestData) malloc(sizeof(*data));
    data->fd = fd;
    data->entry_time = entry_time;
}

void addRequest(requestHandler handler , requestData data){
    pthread_mutex_lock(&handler->mutex);
    while (handler->current_size == handler->max_capacity){
        pthread_cond_wait(&handler->enq,&handler->mutex);
    }
    handler->current_size++;
    enqueue(handler->waiting,data);
    pthread_cond_signal(&handler->deq);
    pthread_mutex_unlock(&handler->mutex);
}

void deleteRequest(requestHandler handler , Node node){
    pthread_mutex_lock(&handler->mutex);
    handler->current_size--;
    deleteNodeQueue(handler->working,node);
    pthread_cond_signal(&handler->enq);
    pthread_mutex_unlock(&handler->mutex);
}

Node executeRequest(requestHandler handler){
    pthread_mutex_lock(&handler->mutex);
    while (handler->waiting->size == 0){
        pthread_cond_wait(&handler->deq,&handler->mutex);
    }
    requestData data = dequeue(handler->waiting);
    enqueue(handler->working,data);
    Node node = handler->working->first;
    pthread_mutex_unlock(&handler->mutex);
    return node;
}

void decreaseCurrentSize(requestHandler handler){
    handler->current_size--;
}