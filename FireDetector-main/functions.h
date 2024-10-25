#ifndef FUNCTION_DEFINITIONS_H
#define FUNCTION_DEFINITIONS_H

#include <pthread.h>

#define QUEUE_CAPACITY 100

typedef struct {
    int x, y;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Sensor;

void setup_terrain(char terrain[30][30]);
void* sensor_handler(void* arg);
void* fire_creator(void* arg);
void* control_station(void* arg);
void extinguish_fire(int x, int y, char terrain[30][30]);
void display_terrain(char terrain[30][30]);
void* terrain_updater(void* arg);

typedef struct {
    int x[QUEUE_CAPACITY];
    int y[QUEUE_CAPACITY];
    int front;
    int rear;
    int count;
} MessageQueue;

void add_message(int x, int y, MessageQueue* queue);
int remove_message(int* x, int* y, MessageQueue* queue);

#endif
