#ifndef OS_WET3_REQUEST_QUEUE
#define OS_WET3_REQUEST_QUEUE


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
}* Queue;

Queue createQueue();
int getQueueSize(Queue queue);
void enqueue(Queue queue , void* data);
void* dequeue(Queue queue);
void* headQueue(Queue queue);
void deleteNodeQueue(Queue queue, Node node);
Node getNodeByIndex(Queue queue, int index);

#endif