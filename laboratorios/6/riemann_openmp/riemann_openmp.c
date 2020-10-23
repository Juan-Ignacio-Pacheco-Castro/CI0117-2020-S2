#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct timespec walltime_t;

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

/**
 * @brief Separates the rectangles between the threads
 * @return
 */
int calculate_riemann_aprox(const size_t a, const size_t upper_limit, const size_t total_rectangles, const size_t workers){
    walltime_t* walltime = (walltime_t*)calloc(1, sizeof (walltime_t));

    double delta = (upper_limit - a)/(double)total_rectangles;
    double area = 0.0;
    double height = 0.0;

    walltime_start(walltime);

    #pragma omp parallel for num_threads(workers) reduction(+:area) default(none) shared(delta) private(height)
    for (size_t rectangle = 0; rectangle < total_rectangles; ++rectangle){
        if (rectangle*delta + a <= upper_limit){
            height = pow(rectangle*delta + a, 2) + 1;
            area += delta * height;
        }
    }


    double time_taken = walltime_elapsed(walltime);
    printf("Estimated area = %.2lf\nExecution time = %lf s\n", area, time_taken);

    free(walltime);

    return 0;
}

int main(int argc, char* arg[]){

    /// a = under limit
    /// b = upper limit
    /// n = rectangle count
    size_t a = 0, b = 0, n = 0, threads = 0;

    if (argc >= 5){
        a = (size_t)strtoul(arg[1], NULL, 10);
        b = (size_t)strtoul(arg[2], NULL, 10);
        n = (size_t)strtoul(arg[3], NULL, 10);
        threads = (size_t)strtoul(arg[4], NULL, 10);
    }else
        return fprintf(stderr, "Error, invalid number of parameters\n"), 1;

    return calculate_riemann_aprox(a, b, n, threads); // 3, workers

}
