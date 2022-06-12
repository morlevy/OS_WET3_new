#ifndef OS_WET3_REQUEST_QUEUE
#define OS_WET3_REQUEST_QUEUE
#include <pthread.h>


typedef struct node{
    void* data;

    struct node* next;
    struct node* prev;
}* Node;

Node createNode(void* data);

typedef struct queue_t{
    Node first;
    Node last;

    int size;
    pthread_cond_t enq;
    pthread_cond_t deq;
    pthread_mutex_t mutex;
}* Queue;

Queue createQueue();
int getQueueSize(Queue queue);
void enqueue(Queue queue , void* data);
void* dequeue(Queue queue);
void* headQueue(Queue queue);

#endif