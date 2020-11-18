
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "mpi/mpi.h"

bool isPrime(size_t number){
    if ( number < 2 ) return false;
    if ( number == 2 ) return true;
    if ( number % 2 == 0 ) return false;

    size_t limit = (size_t)(double)sqrt(number);
    for ( size_t i = 3, last = limit; i <= last; i += 2 )
        if ( number % i == 0 )
            return false;

    return true;
}

int main(int argc, char *argv[]) {

    int myRank, numProcesses, counter = 0, totalPrimes = 0;
    int begin, end;
    long long int limit = 0;
    double startwtime = 0.0, endwtime = 0.0;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if(!myRank){   // myRank = 0
        if (argc >= 2){
            limit = (size_t)strtoul(argv[1], NULL, 10);
            if(limit < 0){
                std::cerr << "Error, limit must be positive\n";
                return -1;
            }
        }else{
            std::cerr << "Error, invalid number of parameters\nUsage: ./count_primes_reduction limit\n";
            return -2;
        }
    }

    // Send limit to other processes
    MPI_Bcast(&limit, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int delta = ceil((limit - 2)/(double)numProcesses);

    if(myRank == 0){
        begin = 2;
        end = begin + delta;
        if(numProcesses != 1)
            MPI_Send(&end, 1, MPI_INT, 1, 123, MPI_COMM_WORLD);
    }else{
        MPI_Recv(&end, 1, MPI_INT, myRank - 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        begin = end + 1;
        end = (begin + delta > limit)?limit:begin + delta;
        if(myRank < numProcesses - 1)
            MPI_Send(&end, 1, MPI_INT, myRank + 1, 123, MPI_COMM_WORLD);
    }

    startwtime = MPI_Wtime();
    for(long long int number = begin; number <= end; ++number){
        if(isPrime(number))
            ++counter;
    }

    MPI_Allreduce(&counter, &totalPrimes, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    endwtime = MPI_Wtime();

    char ready = 'Y';

    if(myRank)
        MPI_Recv(&ready, 1, MPI_CHAR, myRank - 1, 123, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::cout << "process " << myRank << ": " << counter << " out of " << totalPrimes << " primes found in range [" << begin << ", " << end << "] in " << endwtime - startwtime << "s with " << numProcesses << " processes\n";

    if(myRank < numProcesses - 1)
        MPI_Send(&ready, 1, MPI_CHAR, myRank + 1, 123, MPI_COMM_WORLD);

    MPI_Finalize();

    return 0;
}
