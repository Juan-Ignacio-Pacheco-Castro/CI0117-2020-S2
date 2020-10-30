#include <iostream>
#include <omp.h>
#include <string> 
#include <cmath>


using namespace std;

int main(int argc, char *argv[])
{
    if (argc <= 2) {
        cout << "Invalid number of parameters.\n";
        return 1;
    }

    int threads = stoi(argv[1]);
    double sum = 0.0;

    #pragma omp parallel for num_threads(threads) reduction(+:sum)
    for (int i = 2; i < argc; ++i){
        sum += stoi(argv[i]);
        //std::cout << "Threads: " << omp_get_thread_num() << ": " << stoi(argv[i]) << std::endl;
    }

    cout << "Average: " << (sum /(double)(argc - 2)) << endl;
    
    return 0;
}
