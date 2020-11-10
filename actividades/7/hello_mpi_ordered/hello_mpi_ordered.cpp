#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[]) {

    int my_id, num_processes;

    MPI_Init(&argc, &argv);

    MPI_Status status;
    int message_sent = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    int total_processes = num_processes-1;
    if(my_id){
        MPI_Recv(&message_sent, 1 /* count*/, MPI_INT, my_id-1 /*source*/, 123 /*message id*/, MPI_COMM_WORLD, &status);
        cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << endl;
        if (my_id != total_processes)
            MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id+1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    } else {
        cout << "Hello. I am process: " << my_id << ". Total processes: " << num_processes << endl;
        MPI_Send(&message_sent, 1 /*count*/, MPI_INT, my_id+1 /*dest*/, 123 /*message id*/, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
