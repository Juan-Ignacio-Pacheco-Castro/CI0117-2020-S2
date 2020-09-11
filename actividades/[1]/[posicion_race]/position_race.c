#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t position;
} shared_data_t;

typedef struct {
    size_t thread_num;
    size_t thread_total;
    shared_data_t* shared_data;

} thread_data_t;


void* helloWorld(void* args) {

    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    size_t thread_total = data->thread_total;
    shared_data_t* shared_data = data->shared_data;

    printf("Thread %zu / %zu : I arrive at position %zu\n", thread_num, thread_total, shared_data->position);

    ++shared_data->position;

    return NULL;
}


int main(int argc, char* arg[]) {
    size_t thread_count = 0;

    if (argc >= 2) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));

    shared_data_t* shared_data = (shared_message_t*)calloc(1, sizeof(shared_data_t));

    shared_data->position = 0;

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].thread_total = thread_count;
        thread_data_list[i].shared_data = shared_data;
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(shared_message);
    free(thread_data_list);

    return 0;
}
