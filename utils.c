#include<stdio.h>

#define SIZE 1024


typedef struct Car{
    int id;
    char direction;
    char arrival_time[9];
    char cross_time[9];
    char wait_time[9];
} Car;

typedef struct Queue{
    Car *cars[SIZE];
    char direction;
    int carCount;
    int front;
    int rear; 
} Queue;


void enqueue(Queue *queue, Car *car){
    queue->rear = (queue->rear + 1) % SIZE;
    queue->cars[queue->rear] = car;
    queue->carCount++;
}
 
Car* dequeue(Queue *queue)
{
    Car *car = queue->cars[queue->front];
    queue->front = (queue->front + 1) % SIZE;
    queue->carCount--;
    return car;
}
