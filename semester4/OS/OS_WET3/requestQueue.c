#include "requestQueue.h"
#include <stdlib.h>

Node createNode(void* data){
    Node new_node = (Node)malloc(sizeof(*new_node));
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

Queue createQueue(){
    Queue queue = (Queue) malloc(sizeof(*queue));
    pthread_mutex_init(&queue->mutex,NULL);
    pthread_cond_init(&queue->enq,NULL);
    pthread_cond_init(&queue->deq,NULL);
    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;
}

int getQueueSize(Queue queue){
    return queue->size;
}

void enqueue(Queue queue , void* data){
    Node new_node = createNode(data);
    queue->size++;

    if(queue->first == NULL){
        queue->first = new_node;
        queue->last = new_node;
        return;
    }

    queue->last->next = new_node;
    new_node->prev = queue->last;
    queue->last = new_node;
}

void* dequeue(Queue queue){
    if (queue->size == 0){
        return NULL;
    }

    Node first = queue->first;
    void* data = first->data;
    queue->first = first->next;
    if(queue->first != NULL){
        queue->first->prev = NULL;
    }
    free(first);
    queue->size--;
    return data;
}

void* headQueue(Queue queue){
    return queue->last;
}

