#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void* vacio(void* args){
}

int main(int argc, char* arg[])
{
	unsigned int T = 1;
	
	if (argc == 2) {
		T = (int) strtoul(arg[1], NULL, 10);
	} else if (argc > 1) {
		printf("Error, invalid number of parameters\n");
		return 1;
	}
	
	pthread_t secondary_thread;
	clock_t start, stop;
	double total;
	double total_min = INFINITY;
	
	for (int test = 0; test < T; test++){
		start = clock();
		pthread_create(&secondary_thread, NULL, vacio, (void*)test);
		stop = clock();
		total = ((double)(stop-start))/CLOCKS_PER_SEC;
		if (total < total_min){ 
			total_min = total;
		}
	}
	
	printf("Minimun thread creation and destruction time was %lf s among %u trials\n", total_min, T);
	return 0;
}






