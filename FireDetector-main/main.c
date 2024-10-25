#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "functions.h"

char landscape[30][30];
pthread_mutex_t landscape_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    setup_terrain(landscape);

    pthread_t sensor_threads[30][30];
    Sensor sensor_nodes[30][30];

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            sensor_nodes[i][j].x = i;
            sensor_nodes[i][j].y = j;
            pthread_mutex_init(&sensor_nodes[i][j].lock, NULL);
            pthread_cond_init(&sensor_nodes[i][j].cond, NULL);
            pthread_create(&sensor_threads[i][j], NULL, sensor_handler, &sensor_nodes[i][j]);
        }
    }

    pthread_t fire_gen_thread;
    pthread_create(&fire_gen_thread, NULL, fire_creator, NULL);

    pthread_t control_thread;
    pthread_create(&control_thread, NULL, control_station, NULL);

    pthread_t terrain_updater_thread;
    pthread_create(&terrain_updater_thread, NULL, terrain_updater, NULL);

    pthread_join(fire_gen_thread, NULL);
    pthread_join(control_thread, NULL);
    pthread_join(terrain_updater_thread, NULL);

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            pthread_join(sensor_threads[i][j], NULL);
        }
    }

    return 0;
}
