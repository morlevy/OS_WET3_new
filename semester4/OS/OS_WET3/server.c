#include "segel.h"
#include "request.h"
#include "requestHandler.h"
#include "thread.h"
#include <stdbool.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  ./server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c
//

// HW3: Parse the new arguments too
void getargs(int *port,int *num_threads , int *max_queue_size,char** policy, int argc, char *argv[])
{
    if (argc < 5) {
	fprintf(stderr, "Usage: %s <port>\n", argv[0]);
	exit(1);
    }
    *port = atoi(argv[1]);
    *num_threads = atoi(argv[2]);
    *max_queue_size = atoi(argv[3]);
    *policy = argv[4];
}

void* threadFunction(void* thread){
    struct timeval* arrival;// = (struct timeval*) malloc(sizeof(*arrival));
    struct timeval* dispatch = (struct timeval*) malloc(sizeof(*dispatch));
    Thread thread1 = (Thread)thread;
    requestHandler handler = thread1->handler;
    Node node;
    int fd;

    while(1){
        node = executeRequest(handler);
        fd = ((requestData)node->data)->fd;
        arrival = ((requestData )node->data)->entry_time;
        gettimeofday(dispatch,NULL);
        dispatch->tv_sec -= arrival->tv_sec;
        dispatch->tv_usec -= arrival->tv_usec;
        thread1->entry_time = arrival;
        thread1->dispatch_time = dispatch;
        requestHandle(fd , thread1);
        Close(fd);
        deleteRequest(handler,node);
    }

}


int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen, num_threads,max_queue_size;
    char* policy;
    struct sockaddr_in clientaddr;

    getargs(&port,&num_threads,&max_queue_size,&policy, argc, argv);

    // 
    // HW3: Create some threads...
    //
    requestHandler handler = createHandler(max_queue_size);
    for(int i = 0 ; i < num_threads ; i++){
        pthread_t pthread;
        Thread thread = createThread(i , handler);
        pthread_create(&pthread , NULL , threadFunction , thread);
    }

    listenfd = Open_listenfd(port);
    requestData data;
    struct timeval* time;
    bool flag;

    while (1) {
	clientlen = sizeof(clientaddr);
    flag = true;
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);
    gettimeofday(time,NULL);
    data = createRequestData(connfd, time);
    pthread_mutex_lock(&handler->mutex);
    while(handler->current_size >= max_queue_size && flag) {
        if (strcmp(policy, "block") == 0) {
            pthread_cond_wait(&handler->enq,&handler->mutex);
        } else if (strcmp(policy, "drop_tail") == 0) {
                Close(connfd);
                flag = false;
        } else if (strcmp(policy, "drop_random") == 0) {
            int number_of_drops = handler->waiting->size * 0.3;
            for (int i = 0; i < number_of_drops; i++) {
                int index = rand() % handler->waiting->size;
                Node node = getNodeByIndex(handler->waiting, index);
                deleteNodeQueue(handler->waiting, node);
            }
        } else if (strcmp(policy, "drop_head") == 0){
            dequeue(handler->waiting);
        }
    }
    pthread_mutex_unlock(&handler->mutex);
    addRequest(handler,data);
	// 
	// HW3: In general, don't handle the request in the main thread.
	// Save the relevant info in a buffer and have one of the worker threads 
	// do the work. 
	//
    }

}


    


 
