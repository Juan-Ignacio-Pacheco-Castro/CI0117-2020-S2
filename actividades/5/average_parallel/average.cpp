#include <iostream>
#include <omp.h>
#include <string> 
#include <cmath>


using namespace std;

static void calculate_sum(char **array, int array_size, double* global_sum, int max_array){
    int local_sum = 0;
    int start_point = array_size*(omp_get_thread_num()) + 2;
    int end_point = array_size*(omp_get_thread_num()) + 2 + array_size;
    //std::cout << "I " << omp_get_thread_num() << " start and end: " << start_point << " " << end_point << std::endl;
    for (int i = start_point; i < end_point && i < (max_array); ++i){
        local_sum += stoi(array[i]);
    }
    #pragma omp critical
    {
    *global_sum += local_sum;
    std::cout << "Thread " << omp_get_thread_num() << ": Processing number " << local_sum << std::endl;
    //std::cout << global_sum << " resultaat" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 2 /*|| ((argc-2)%(stoi(argv[1])) != 0)*/) {
        cout << "Invalid number of parameters.\n";
        return 1;
    }

    int threads = stoi(argv[1]);
    double sum = 0.0;

#if 0
    for (int i = 1; i < argc; i++) {        
        sum += stoi(argv[i]);       
    }
#endif

    double array_size = ceil((argc-2)/(double)threads);

    #pragma omp parallel num_threads(threads)
    {
        calculate_sum(argv, array_size, &sum, argc);
    }

    cout << "Average: " << (sum / (argc - 2)) << endl;
    
    return 0;
}
