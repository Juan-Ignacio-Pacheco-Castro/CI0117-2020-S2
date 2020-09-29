#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct timespec walltime_t;

typedef struct {
    size_t rectangle_total; //Talvez ir restando al rectangle total...
    double b_global;
    double delta;
    double shared_area;
    pthread_rwlock_t mutex;
} shared_data_t;

typedef struct {
    double a;
    double b;
    shared_data_t* shared_data;
} thread_data_t;

void walltime_start(walltime_t* start){
    clock_gettime(CLOCK_MONOTONIC, start);
}

double walltime_elapsed(const walltime_t* start){
    walltime_t finish;
    clock_gettime(CLOCK_MONOTONIC, &finish);

    double elapsed = (finish.tv_sec - start->tv_sec);
    elapsed += (finish.tv_nsec - start->tv_nsec) / 1000000000.0;

    return elapsed;
}

void* calculate_area(void* argument){
    thread_data_t* thread_data = (thread_data_t*)argument;
    shared_data_t* shared_data = thread_data->shared_data;

    double height = 0.0, my_area = 0.0;
    for(double rectangle = thread_data->a; rectangle < thread_data->b && rectangle < shared_data->b_global; rectangle += shared_data->delta){
        height = pow(rectangle, 2) + 1;
        pthread_rwlock_rdlock(&shared_data->mutex);
        my_area += shared_data->delta * height;
        pthread_rwlock_unlock(&shared_data->mutex);
    }

    pthread_rwlock_wrlock(&shared_data->mutex);
    shared_data->shared_area += my_area;
    pthread_rwlock_unlock(&shared_data->mutex);

    return NULL;
}

/**
 * @brief Separates the rectangles between the threads
 * @return
 */
int calculate_riemann_aprox(const size_t a, const size_t b, const size_t n, const size_t workers){
    walltime_t* walltime = (walltime_t*)calloc(1, sizeof (walltime_t));
    walltime_start(walltime);

    double delta = (b - a)/(double)n;           // 0.5
    size_t iterations = ceil(n/(double)workers);      // rectangles for each thread, // 4/8 = 1/2 = 0.5 = 1

    pthread_t* threads = (pthread_t*)malloc((size_t)(workers * sizeof(pthread_t)));
    if (threads == NULL)
        return fprintf(stderr, "Error: could not allocate memory for %zu threads\n", workers), 2;

    shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
    if (shared_data == NULL )
        return (void)fprintf(stderr, "Error: could not allocate shared memory\n"), 3;

    thread_data_t* private_data = (thread_data_t*)malloc((size_t)(workers * sizeof(thread_data_t)));
    if (private_data == NULL )
        return (void)fprintf(stderr, "Error: could not allocate thread data for %zu threads\n", workers), 4;

    pthread_rwlock_init(&shared_data->mutex, NULL);

    shared_data->delta = delta;
    shared_data->shared_area = 0.0;
    shared_data->rectangle_total = n;
    shared_data->b_global = b;
    double under = a;

    for (size_t thread = 0; thread < workers; ++thread){
        private_data[thread].shared_data = shared_data;
        private_data[thread].a = under;
        private_data[thread].b = under + iterations*delta;
        pthread_create(&threads[thread], NULL, calculate_area, (void*)&private_data[thread]);
        under += iterations*delta;
    }

    for (size_t thread = 0; thread < workers; ++thread)
        pthread_join(threads[thread], NULL);

    double time_taken = walltime_elapsed(walltime);
    printf("Estimated area = %.2lf\nExecution time = %lf s\n", shared_data->shared_area, time_taken);

    pthread_rwlock_destroy(&shared_data->mutex);
    free(private_data);
    free(shared_data);
    free(threads);
    free(walltime);

    return 0;
}

int main(int argc, char* arg[]){

    /// a = under limit
    /// b = upper limit
    /// n = rectangle count
    size_t a = 0, b = 0, n = 0, workers = 0;

    if (argc >= 5){
        a = (size_t)strtoul(arg[1], NULL, 10);
        b = (size_t)strtoul(arg[2], NULL, 10);
        n = (size_t)strtoul(arg[3], NULL, 10);
        workers = (size_t)strtoul(arg[4], NULL, 10);
    }else
        return fprintf(stderr, "Error, invalid number of parameters\n"), 1;

    //if(n % workers)
    //    return fprintf(stderr, "Rectangle count must be divisible by workers threads\n"), 5;

    return calculate_riemann_aprox(a, b, n, workers); // 3, workers

}
