#include <stdio.h>
#include <pthread.h>
#include "pthread_sleep.c"

void *print_hello(void *ptr);

int main(){
    pthread_t lane_queues[4];

    for(int i=0; i<4; i++){
        pthread_create(&lane_queues[i], NULL, print_hello, (void*) i);
    }

    for(int i=0; i<4; i++){
        pthread_join(lane_queues[i], NULL);
    }
}


void *print_hello(void *ptr){
    int sleep = (int) ptr;
    pthread_sleep(sleep);
    printf("%s%d\n", "hello", sleep);
}