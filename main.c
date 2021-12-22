#include <stdio.h>
#include <pthread.h>
#include "pthread_sleep.c"
#include "utils.c"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t north_pass_condition;
pthread_cond_t east_pass_condition;
pthread_cond_t south_pass_condition;
pthread_cond_t west_pass_condition;

pthread_cond_t iteration_finish_condition;
pthread_cond_t police_work_condition;
pthread_t lane_threads[4];

void *lane(void* condition_ptr){
    pthread_mutex_lock(&lock);
    pthread_cond_wait(condition_ptr, &lock);
    //Implementation here
    printf("Hello world\n");

    pthread_cond_signal(&iteration_finish_condition);
    pthread_mutex_unlock(&lock);
}

void *police_officer_function(){
    pthread_mutex_lock(&lock);
    pthread_mutex_wait(&police_work_condition);

    
    pthread_cond_signal(&north_pass_condition);
    pthread_mutex_unlock(&lock);
    sleep(1);
}

int main(int argc, char const *argv[]){
    pthread_t lane_queues[4];
    // Args: -s timeLog(arg1) simulationTime(arg2) probability(arg3) seed(arg4)

    //simulationTime

    int time = 0;
    time = atoi(argv[2]);

    //get probability
    float prob = 0.0;
    prob = atof(argv[3]);

    //get seed
    int seed = 0;


    pthread_t police_officer_thread;

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&north_pass_condition, NULL);
    pthread_cond_init(&south_pass_condition, NULL);
    pthread_cond_init(&east_pass_condition, NULL);
    pthread_cond_init(&west_pass_condition, NULL);


    pthread_create(&lane_threads[0], NULL, lane, (void *) &north_pass_condition);
    pthread_create(&lane_threads[1], NULL, lane, (void *) &west_pass_condition);
    pthread_create(&lane_threads[2], NULL, lane, (void *) &south_pass_condition);
    pthread_create(&lane_threads[3], NULL, lane, (void *) &east_pass_condition);

    pthread_create(&police_officer_thread, NULL, police_officer_function, NULL);

    int i=0;
    while(i <= time){
        pthread_mutex_lock(&lock);
        pthread_cond_signal(&police_work_condition);
        pthread_mutex_unlock(&lock);

        i++;
    }

    return 0;
}
