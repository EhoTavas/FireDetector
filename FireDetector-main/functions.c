#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

extern char landscape[30][30];
extern pthread_mutex_t landscape_mutex;
MessageQueue message_queue = { .front = 0, .rear = 0, .count = 0 };

void setup_terrain(char terrain[30][30]) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            int random_val = rand() % 10;
            if (random_val < 7) {
                terrain[i][j] = 'T';
            } else {
                terrain[i][j] = '-';
            }
        }
    }
}

void* sensor_handler(void* arg) {
    Sensor* node = (Sensor*) arg;

    while (1) {
        pthread_mutex_lock(&node->lock);
        if (landscape[node->x][node->y] == '@') {
            printf("Sensor at (%d, %d) detected fire! Notifying...\n", node->x, node->y);
            
            if (node->x == 0 || node->x == 29 || node->y == 0 || node->y == 29) {
                add_message(node->x, node->y, &message_queue);
            } else {
                extinguish_fire(node->x, node->y, landscape);
            }
        }
        pthread_mutex_unlock(&node->lock);
        sleep(2);
    }
    return NULL;
}

void extinguish_fire(int x, int y, char terrain[30][30]) {
    pthread_mutex_lock(&landscape_mutex);
    if (terrain[x][y] == '@') {
        printf("Extinguishing fire at (%d, %d), cell burned!\n", x, y);
        terrain[x][y] = '/';
    }
    pthread_mutex_unlock(&landscape_mutex);
}

void* fire_creator(void* arg) {
    while (1) {
        int x = rand() % 30;
        int y = rand() % 30;

        pthread_mutex_lock(&landscape_mutex);
        if (landscape[x][y] == 'T') {
            landscape[x][y] = '@';
            printf("\n*** Fire started at (%d, %d) ***\n", x, y);
        }
        pthread_mutex_unlock(&landscape_mutex);

        sleep(5);
    }
    return NULL;
}

void* control_station(void* arg) {
    int x, y;

    while (1) {
        if (remove_message(&x, &y, &message_queue) == 1) {
            printf("Control center extinguishing fire at (%d, %d)\n", x, y);
            extinguish_fire(x, y, landscape);
        }
        sleep(1);
    }
    return NULL;
}

void display_terrain(char terrain[30][30]) {
    printf("\nCurrent State of the Landscape:\n");
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            printf("%c ", terrain[i][j]);
        }
        printf("\n");
    }
}

void* terrain_updater(void* arg) {
    while (1) {
        display_terrain(landscape);
        sleep(5);
    }
    return NULL;
}

void add_message(int x, int y, MessageQueue* queue) {
    pthread_mutex_lock(&landscape_mutex);
    if (queue->count < QUEUE_CAPACITY) {
        queue->x[queue->rear] = x;
        queue->y[queue->rear] = y;
        queue->rear = (queue->rear + 1) % QUEUE_CAPACITY;
        queue->count++;
    }
    pthread_mutex_unlock(&landscape_mutex);
}

int remove_message(int* x, int* y, MessageQueue* queue) {
    pthread_mutex_lock(&landscape_mutex);
    if (queue->count > 0) {
        *x = queue->x[queue->front];
        *y = queue->y[queue->front];
    }