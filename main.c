#include <stdio.h>
#include <pthread.h>
#include "pthread_sleep.c"
#include "utils.c"
#include <time.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t north_pass_condition;
pthread_cond_t east_pass_condition;
pthread_cond_t south_pass_condition;
pthread_cond_t west_pass_condition;

pthread_cond_t iteration_finish_condition;
pthread_t lane_threads[4];
Queue queueN;
Queue queueS;
Queue queueE;
Queue queueW;
time_t Time;
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
    Car *car;
    ID++;
    switch (direction)
    {
    case 'N':
        car->id = ID;
        car->direction = 'N';
        car->arrival_time[0] = Time;
        
        break;
    case 'E':
        car->id = ID;
        car->direction = 'E';
        car->arrival_time[0] = Time;
        
        break;
    case 'S':
        car->id = ID;
        car->direction = 'S';
        car->arrival_time[0] = Time; //TODO CREATE CURRENT TIMES
        
        break;
    case 'W':
        car->id = ID;
        car->direction = 'W';
        car->arrival_time[0] = Time;
        
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
    
    Car *car;
    if (Nprob >= p) 
    {
        car = createCar('N'); //Todo: keep track of 20 sec and add it definitely after that
    } 

    if (Sprob <= p)
    {
        car = createCar('S');
    }
    if (Wprob <= p)
    {
        car = createCar('W');
    }
    if (Eprob <= p)
    {
        car = createCar('E');
    }
    
    

}

void initializeLaneQueues() {
    //initialize queue lanes
    Car car;
    car.arrival_time[0] = '0';
    car.id = '0'; //TODO : HOW TO SET ID   
    queueN.direction = 'N';
    enqueue(&queueN, car);
    queueN.carCount ++;

    queueS.direction = 'S';
    car.id = '1'; //change id to use again
    enqueue(&queueS, car);
    queueS.carCount ++;
    

    queueE.direction = 'E';
    car.id = '2'; //change id to use again
    enqueue(&queueE, car);
    queueE.carCount ++;

    queueW.direction = 'W';
    car.id = '3'; //change id to use again
    enqueue(&queueW, car);
    queueW.carCount ++;

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
        printf("%d", seed);

        //set seed
        srand(seed);

        Time = time(NULL);
    //initialize log file
    FILE *carLog;
    FILE *policeLog;
    carLog = fopen("car.log", "+w");
    fprintf(carLog,"CarID\tDirection\tArrival-Time\tCross-Time\tWait-Time \n");
    fprintf(carLog,"----------------------------------------------------------------------------------------------------------------");

    policeLog = fopen("police.log", "+w");
    fprintf(policeLog,"Time\tEvent\n");
    fprintf(policeLog,"---------------------------------------------------------------------------");







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

    pthread_join(police_officer_thread, NULL);

    return 0;
}



