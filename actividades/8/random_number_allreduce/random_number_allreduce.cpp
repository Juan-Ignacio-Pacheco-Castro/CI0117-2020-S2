#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

int main(int argc, char *argv[]) {

    int my_id, num_processes;

    MPI_Init(&argc, &argv);

    MPI_Status status;
    int message_sent = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    srand(time(NULL) * my_id * 1000);

    int cant_enteros = 0;
    if (my_id == 0){
        std::cout << "Inserte la cantidad de enteros a ingresar: ";
        std::cin >> cant_enteros;
        std::cout << "\n";

        if (cant_enteros <= 0) return 1;
    }


    MPI_Bcast(&cant_enteros, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int* arreglo_enteros = new int[cant_enteros];

    if (my_id == 0){
        for (int index = 0; index < cant_enteros; index++){
            std::cout << "Ingrese un valor: ";
            std::cin >> arreglo_enteros[index];
            std::cout << "\n";
        }
    }
    MPI_Bcast(arreglo_enteros, cant_enteros, MPI_INT, 0, MPI_COMM_WORLD);

    int index = rand() % cant_enteros;
    int message_received = 0;
    int message_send = 0;

    int mpi_max = 0;
    int mpi_min = 0;
    int mpi_sum = 0;
    double mpi_avg = 0;
    MPI_Allreduce(&arreglo_enteros[index], &mpi_max, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&arreglo_enteros[index], &mpi_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&arreglo_enteros[index], &mpi_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    mpi_avg = mpi_sum/(double)num_processes;

    if (my_id != 0) {
            MPI_Recv(&message_received, 1 /* count*/, MPI_INT, my_id - 1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
    }

    std::cout << "Process: " << my_id << ": I chose number " << arreglo_enteros[index];
    if (arreglo_enteros[index] == mpi_min){
        std::cout << ". It's  the minimun";
    }
    if (arreglo_enteros[index] == mpi_max){
        std::cout << ". It's  the maximun";
    }
    if (arreglo_enteros[index] < mpi_avg){
        std::cout << " and it's less than the average (" << mpi_avg << ")" << std::endl;
    } else if (arreglo_enteros[index] > mpi_avg){
        std::cout << " and it's more than the average (" << mpi_avg << ")" << std::endl;
    } else {
        std::cout << " and it's equal than the average (" << mpi_avg << ")" << std::endl;
    }


    if ( my_id < num_processes - 1 ) {
        MPI_Send(&message_send, 1 /*count*/, MPI_INT, my_id + 1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }


    delete [] arreglo_enteros;
    MPI_Finalize();


    return 0;
}
