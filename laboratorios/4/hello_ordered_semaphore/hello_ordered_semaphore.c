#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct {
    size_t message;
    size_t chose_thread;
    size_t next_thread;
    sem_t* semaphores;
} shared_message_t;

typedef struct {
    size_t thread_num;
    shared_message_t* shared_message;
} thread_data_t;


void* helloWorld(void* args) {
    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_message_t* shared_message = data->shared_message;

	//Hacer lock. Manda a todos los threads a dormir
	sem_wait(&shared_message->semaphores[thread_num]);
	
	if (thread_num == shared_message->chose_thread) {
        shared_message->message = 2021;
    }

    printf("Hello world from thread number # %zu. The message is: %zu\n", thread_num, shared_message->message);
	
    ++shared_message->next_thread;
    //Unlock al siguiente thread
    sem_post(&shared_message->semaphores[shared_message->next_thread]);
    
    //Hacer unlock a este thread
    sem_post(&shared_message->semaphores[thread_num]);
    sem_destroy(&shared_message->semaphores[thread_num]);
    return NULL;
}


int main(int argc, char* arg[]) {

    size_t thread_count = 0;
    size_t chosen_thread = 0;

    if (argc >= 3) {
        thread_count = (size_t)strtoul(arg[1], NULL, 10);
        chosen_thread = (size_t)strtoul(arg[2], NULL, 10);

        if (chosen_thread < 0 || chosen_thread >= thread_count) {
            fprintf(stderr, "Error, invalid parameters\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Error, invalid number of parameters\n");
        return 1;
    }

    pthread_t* threads = malloc((size_t)(thread_count * sizeof(pthread_t)));
	
    shared_message_t* shared_message = (shared_message_t*)calloc(1, sizeof(shared_message_t));

    shared_message->message = 2020;
    shared_message->chose_thread = chosen_thread;
    shared_message->next_thread = 0;
    
    //Declaro array de semaforos y la meto en Share_message
    //pthread_mutex_t* mutexes = callocalloc((size_t)(thread_count * sizeof(pthread_mutex_t)));
    sem_t* semaphores = (sem_t*)calloc(thread_count, sizeof(sem_t));
    shared_message->semaphores = semaphores;
 
    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_message = shared_message;
        
        //inicializo mutexes
        sem_init(&semaphores[i], 0, 0);
        if ( i == 0 ) {
			//lock a todos los mutexes excepto el primero
			sem_post(&semaphores[i]);
		}
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Hello world from main thread\n");

    free(threads);
    free(shared_message->semaphores);
    free(shared_message);
    //free(thread_data_list);

    return 0;
}

//Un arreglo de semaforos, uno por cada thread
