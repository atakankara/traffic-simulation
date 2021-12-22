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
pthread_cond_t laneConditions[4];

pthread_cond_t iteration_finish_condition;
pthread_cond_t police_work_condition;
pthread_t lane_threads[4];

Queue *queues[4]; // {N, E, S, W}

char currentLane = 'N';

FILE *carLog;
FILE *policeLog;
char currentTimeString[8];
int ID = 0;

int checkMoreThanFiveCar(){
    for(int i=0; i<4; i++){
        if(queues[0]->carCount >= 5){
            return 1;
        }
    }
    return 0;
}

int checkIfAllLinesEmpty(){
    for(int i=0; i<4; i++){
        if(queues[0]->carCount > 0){
            return 1;
        }
    }
    return 0;
}

int getTheMostCrowdedLane(){
    int currentCount = 0;
    int lane;
    for(int i=3; i>=0; i--){
        if(queues[i]->carCount >= currentCount){
            lane = i;
        }
    }
    return lane;
}

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

    if(checkIfAllLinesEmpty()){
        pthread_mutex_unlock(&lock);
        return 0;
    }

    if(checkMoreThanFiveCar()){
        currentLane = getTheMostCrowdedLane();
        pthread_cond_signal(&laneConditions[currentLane]);

    }else if(queues[currentLane]->carCount == 0){
        //N>E>S>W
        for(int i=0; i<4; i++){
            if(queues[i]->carCount != 0){
                currentLane=i;
                pthread_cond_signal(&laneConditions[currentLane]);
                break;
            }
        }
    }else{
        //don't change current line
        pthread_cond_signal(&laneConditions[currentLane]);
    }
    pthread_mutex_unlock(&lock);

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
        queues[i] = (struct Queue*) malloc(sizeof(Queue));
    }

    //at t=0 all lanes have a car
    enqueue(queues[0], createCar('N'));

    enqueue(queues[1], createCar('E'));
    
    enqueue(queues[2], createCar('S'));

    enqueue(queues[3], createCar('W'));

}
int getWaitTime(Car *car){
    int waitTime = 0;
    char delim[] = ":";

    int arrival_h = atoi(strtok(car->arrival_time, delim));
    int arrival_m = atoi(strtok(NULL, delim));
    int arrival_s = atoi(strtok(NULL, delim));

    int cross_h = atoi(strtok(car->cross_time, delim));
    int cross_m = atoi(strtok(NULL, delim));
    int cross_s = atoi(strtok(NULL, delim));

    int h = cross_h - arrival_h;
    int m = cross_m - arrival_m;
    int s = cross_s - arrival_s;

    waitTime = h * 3600 + m * 60 + s;

    return waitTime;
}

void updateLogFile(Car *car){
    //Add car to log file //CarID Direction Arrival-Time Cross-Time Wait-Time
    char logMsg[100];
    sprintf(logMsg, "%d\t%c\t%s\t%s\t%s", car->id, car->direction, car->arrival_time, car->cross_time, getWaitTime(car));
    fprintf(carLog, "%s", logMsg);
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

    carLog = fopen("car.log", "w");
    policeLog = fopen("police.log", "w");
    if(carLog == NULL && policeLog == NULL)
    {
        perror("Program crashed.\n");
        exit(1);
    }
    fprintf(carLog,"CarID\tDirection\tArrival-Time\tCross-Time\tWait-Time \n");
    fprintf(carLog,"----------------------------------------------------------------------------------------------------------------\n");

    fprintf(policeLog,"Time\tEvent\n");
    fprintf(policeLog,"-----------------------------------------------------------------------------------------------\n");



    pthread_t police_officer_thread;

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&lock, NULL);

    for(int i=0; i<4; i++){
        pthread_cond_init(&laneConditions[i], NULL);
    }

    pthread_cond_init(&police_work_condition, NULL);
    pthread_cond_init(&iteration_finish_condition, NULL);



    for(int i=0; i<4; i++){
        pthread_create(&lane_threads[0], NULL, lane, (void *) &laneConditions[i]);
    }

    pthread_create(&police_officer_thread, NULL, police_officer_function, NULL);

    int i=0;
    while(i <= simulationTime){
        pthread_mutex_lock(&lock);
        pthread_cond_signal(&police_work_condition);
        pthread_mutex_unlock(&lock);

        pthread_mutex_lock(&lock);
        pthread_cond_wait(&iteration_finish_condition, &lock);
        addCar(prob);
        i++;
        printf("On iteration %d\n", i);
}

    return 0;
}



