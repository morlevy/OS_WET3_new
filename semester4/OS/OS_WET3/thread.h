#ifndef OS_WET3_THREAD
#define OS_WET3_THREAD
#include "requestHandler.h"

typedef struct thread{
    int num_of_requests;
    int num_of_dynamic_requests;
    int num_of_static_requests;
    requestHandler handler;
    int id;
    struct timeval* entry_time;
    struct timeval* dispatch_time;
}*Thread;

Thread createThread(int id , requestHandler handler);

#endif