#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;
    srand(time(NULL));
    MPI_Init(&argc, &argv);

    MPI_Status status;
    int message_sent = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int total_processes = num_processes-1;
    if(!my_id){
        while (total_processes){
            total_processes--;
            MPI_Recv(&message_sent, 1 /* count*/, MPI_INT, MPI_ANY_SOURCE /*source*/, MPI_ANY_TAG /*message id*/, MPI_COMM_WORLD, &status);
            cout << "Process: " << status.MPI_SOURCE << " finished. Tag accessed: " << status.MPI_TAG << " Total processes: " << num_processes << endl;
        }
    } else {
        //message_sent = my_id;
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, 0 /*dest*/, rand() % 100/*message id*/, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
