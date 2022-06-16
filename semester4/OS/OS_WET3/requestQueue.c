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
    new_node->prev = queue->last;
    queue->last->next = new_node;
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

void deleteNodeQueue(Queue queue , Node node){
    if(node->prev != NULL){
        queue->size--;
        node->prev->next = node->next;
        if(node->next == NULL){
            queue->last = node->prev;
        } else {
            node->next->prev = node->prev;
        }
        free(node);
    }
    else{
        dequeue(queue);
    }
}

Node getNodeByIndex(Queue queue, int index){
    Node current = queue->first;
    for(int i = 0; i < index;i++){
        current = current->next;
    }
    return current;
}