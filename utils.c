#include<stdio.h>

#define SIZE 1024


typedef struct Car{
    int id;
    char direction;
    char arrival_time[9];
    char cross_time[9];

} Car;

typedef struct Queue{
    Car *cars[SIZE];
    char direction;
    int carCount;
    int front;
    int rear;
} Queue;


void enqueue(Queue *queue, Car *car){
    if (queue->rear == SIZE - 1)
        printf("\nQueue is Full!!");
    else {
        if (queue->front == -1)
            queue->front = 0;
        queue->rear++;
        queue->cars[queue->rear] = car;
        queue->carCount ++;
    }
}
 
Car* dequeue(Queue *queue)
{
    Car *car;

    if (queue->front == -1) {
        printf("\nDeque is empty.\n");
        return 0;
    }

    car = queue->cars[queue->front];
    queue->cars[queue->front] = NULL;
    (queue->front)++;
    queue->carCount --;

    if (queue->front > queue->rear)
        queue->front = queue->rear = -1;



//    return car;
//    Car *car = queue->cars[queue->front];
//    queue->front = (queue->front + 1) % SIZE;
//    queue->carCount--;
    return car;
}
