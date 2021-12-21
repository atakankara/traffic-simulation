#include <stdio.h>
#include <pthread.h>
#include "pthread_sleep.c"

void *print_hello(void *ptr);

int main(int argc, char const *argv[]){
    pthread_t lane_queues[4];
    // Args: timeLog(arg1) simulationTime(arg2) probability(arg3) seed(arg4)

        //simulationTime

        int time = 0;
        time = atoi(argv[2]);

        //get probability
        float prob = 0.0;
        prob = atof(argv[3]);

        //get seed
        int seed = 0;

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
