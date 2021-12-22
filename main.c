#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pthread_sleep.c"
#include "utils.c"


char* getCurrentTime();


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t north_pass_condition;
pthread_cond_t east_pass_condition;
pthread_cond_t south_pass_condition;
pthread_cond_t west_pass_condition;

pthread_cond_t iteration_finish_condition;
pthread_cond_t police_work_condition;
pthread_t lane_threads[4];

Queue *queues[4]; // {N, E, S, W}

char currentTimeString[8];
int ID = 0;


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
    pthread_cond_wait(&police_work_condition, &lock);


    pthread_cond_signal(&north_pass_condition);
    pthread_mutex_unlock(&lock);
    sleep(1);

    pthread_mutex_lock(&lock);
    pthread_cond_signal(&west_pass_condition);
    pthread_mutex_unlock(&lock);
    sleep(1);

    pthread_mutex_lock(&lock);
    pthread_cond_signal(&south_pass_condition);
    pthread_mutex_unlock(&lock);
    sleep(1);

    pthread_mutex_lock(&lock);
    pthread_cond_signal(&east_pass_condition);
    pthread_mutex_unlock(&lock);
    sleep(1);
}

Car *createCar(char direction) {
    Car *car = (struct Car*) malloc(sizeof(Car));

    ID++;
    car->id = ID;
    strcpy(car->arrival_time, getCurrentTime());

    switch (direction){
    case 'N':
        car->direction = 'N';
        break;
    case 'E':
        car->direction = 'E';
        break;
    case 'S':
        car->direction = 'S';
        break;
    case 'W':
        car->direction = 'W';
        break;
    default:
        break;
    }
    return car;
}

void addCar(double p) {

    //implement car struct -
    //implement queue struct -
    //create cars with random probability  

    double Nprob = rand() % 100;
    double Sprob = rand() % 100;
    double Wprob = rand() % 100;
    double Eprob = rand() % 100;

    p = p*100;

    Car *car;
    if (Nprob > p)
    {
        enqueue(queues[0], createCar('N')); //Todo: keep track of 20 sec and add it definatly after that
    } 

    if (Eprob <= p)
    {
        enqueue(queues[1], createCar('E'));
    }
    if (Sprob <= p)
    {
        enqueue(queues[2], createCar('S'));
    }
    if (Wprob <= p)
    {
        enqueue(queues[3], createCar('W'));
    }

}

void initializeLaneQueues() {
    //initialize queue lanes
    for(int i = 0; i < 4; i++){
        queues[i] = malloc((struct Queue*) malloc(sizeof(Queue)));
    }

    //at t=0 all lanes have a car
    enqueue(queues[0], createCar('N'));

    enqueue(queues[1], createCar('E'));
    
    enqueue(queues[2], createCar('S'));

    enqueue(queues[3], createCar('W'));

}

char* getCurrentTime(){
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    sprintf(currentTimeString, "%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    return currentTimeString;
}


int main(int argc, char const *argv[]){
    pthread_t lane_queues[4];
    // Args: -s timeLog(arg1) simulationTime(arg2) probability(arg3) seed(arg4)

    //simulationTime

    int simulationTime = 0;
    simulationTime = atoi(argv[2]);

        //get probability
        double prob = 0.0;
        prob = atof(argv[3]);

        //get seed
        int seed = 0;
        seed = atoi(argv[4]);

        //set seed
        srand(seed);
    //initialize log file
    FILE *carLog;
    FILE *policeLog;
    carLog = fopen("car.log", "w");
    policeLog = fopen("police.log", "w");
    if(carLog == NULL && policeLog == NULL)
    {
        perror(carLog);
        exit(1);
    }
    fprintf(carLog,"CarID\tDirection\tArrival-Time\tCross-Time\tWait-Time \n");
    fprintf(carLog,"----------------------------------------------------------------------------------------------------------------\n");

    fprintf(policeLog,"Time\tEvent\n");
    fprintf(policeLog,"-----------------------------------------------------------------------------------------------\n");



    pthread_t police_officer_thread;

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&north_pass_condition, NULL);
    pthread_cond_init(&south_pass_condition, NULL);
    pthread_cond_init(&east_pass_condition, NULL);
    pthread_cond_init(&west_pass_condition, NULL);
    pthread_cond_init(&police_work_condition, NULL);

    pthread_create(&lane_threads[0], NULL, lane, (void *) &north_pass_condition);
    pthread_create(&lane_threads[1], NULL, lane, (void *) &west_pass_condition);
    pthread_create(&lane_threads[2], NULL, lane, (void *) &south_pass_condition);
    pthread_create(&lane_threads[3], NULL, lane, (void *) &east_pass_condition);

    pthread_create(&police_officer_thread, NULL, police_officer_function, NULL);

    int i=0;
    while(i <= simulationTime){
        // pthread_mutex_lock(&lock);
        // pthread_cond_signal(&police_work_condition);
        // pthread_mutex_unlock(&lock);

        i++;
}

    return 0;
}



