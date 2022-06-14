#include "thread.h"
#include <stdlib.h>

Thread createThread(int id , requestHandler handler){
    Thread thread = (Thread) malloc(sizeof(*thread));
    thread->handler = handler;
    thread->id = id;
    thread->entry_time = NULL;
    thread->dispatch_time = NULL;
    thread->num_of_dynamic_requests = 0;
    thread->num_of_requests = 0;
    thread->num_of_static_requests = 0;
    return thread;
}