#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t message;
    size_t chose_thread;
    size_t next_thread;
    pthread_mutex_t* mutexes;
} shared_message_t;

typedef struct {
    size_t thread_num;
    shared_message_t* shared_message;

} thread_data_t;


void* helloWorld(void* args) {
    thread_data_t* data = (thread_data_t*) args;

    size_t thread_num = data->thread_num;
    shared_message_t* shared_message = data->shared_message;
    pthread_mutex_t mutex_position = shared_message->mutexes[thread_num];

    //while (thread_num != shared_message->next_thread);

    /*if (thread_num == shared_message->chose_thread) {
        shared_message->message = 2021;
    }*/

	//Hacer lock. Manda a todos los threads a dormir
	//pthread_mutex_lock(&(shared_message->mutexes[thread_num]));

    printf("Hello world from thread number # %zu. The message is: %zu\n", thread_num, shared_message->message);
 
    ++shared_message->next_thread;
    //Hacer unlock
    //pthread_mutex_unlock(&shared_message->mutexes[thread_num]);
    //pthread_mutex_unlock(&shared_message->mutexes[shared_message->next_thread]);
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
    
    //Declaro lista de mutexes
    pthread_mutex_t* mutexes = malloc((size_t)(thread_count * sizeof(pthread_mutex_t)));
    

    thread_data_t* thread_data_list = malloc((size_t)(thread_count * sizeof(thread_data_t)));

    for (size_t i = 0; i < thread_count; ++i) {
        thread_data_list[i].thread_num = i;
        thread_data_list[i].shared_message = shared_message;
        
        //inicializo mutexes
        pthread_mutex_init(&mutexes[i], /*attr*/ NULL);
        if ( i != 0 ) {
			//lock a todos los mutexes excepto el primero
			pthread_mutex_lock(&mutexes[i]);
		}
        pthread_create(&threads[i], NULL, helloWorld, (void*)&thread_data_list[i]);
    }

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Hello world from main thread\n");

    free(threads);
    free(shared_message);
    free(thread_data_list);

    return 0;
}

//Obligar a los hilos ejecutarse en orden en la salida estandar
//Coleccion de mutexes, uno por thread. -> Declarar en el for que se declaran mutexes
//Solo el thread 0 tiene el mutex habilitado, los demas se inicializan locked
//Cuando el thread 0 ha saludado habilita el siguiente mutex
//Correr solucion y medir tiempo ejecucion
