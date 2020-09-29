
#include <math.h>
#include <pthread.h>
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

void calculate_riemann_aprox(const size_t a, const size_t b, const size_t n){
    walltime_t* walltime = (walltime_t*)calloc(1, sizeof (walltime_t));
    walltime_start(walltime);

    // delta = (a - b)/n
    // height = f(left extreme)
    double delta = (b - a)/(double)n, height = 0;

    double area = 0.0;
    for(double rectangle = a; rectangle < b; rectangle += delta){
        height = pow(rectangle, 2) + 1;
        area += delta * height;
    }

    double time_taken = walltime_elapsed(walltime);
    printf("Estimated area = %.2lf\nExecution time = %lf s\n", area, time_taken);

    free(walltime);
}

int main(int argc, char* arg[]){

    /// a = upper limit
    /// b = under limit
    /// n = rectangle count
    size_t a = 0, b = 0, n = 0;

    if (argc >= 4){
        a = (size_t)strtoul(arg[1], NULL, 10);  // 3
        b = (size_t)strtoul(arg[2], NULL, 10);  // 1
        n = (size_t)strtoul(arg[3], NULL, 10);  // 4
    }else
        return fprintf(stderr, "Error, invalid number of parameters\n"), 1;

    calculate_riemann_aprox(a, b, n);

    return 0;
}
