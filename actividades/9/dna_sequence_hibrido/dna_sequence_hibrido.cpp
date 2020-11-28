#include <cmath>
#include <fstream>
#include <iostream>
#include "mpi/mpi.h"
#include <omp.h>
#include <string>

using namespace std;

void initArray(int *array, int size)
{
    for (int i = 0; i < size; ++i)
    {
        array[i] = 0;
    }
}

void throwError(const int error, const std::string strError){
    char go = 'N';
    MPI_Bcast(&go, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    std::cerr << strError << std::endl;
    throw error; // THROW
}

int validateInputData(const int &myRank, const int argc, char* argv[], int& segmentSize, string& sequence, const int numProcesses){
    char go = 'Y';
    if (myRank == 0){
        if (argc < 2) {
            throwError(1, "Invalid number of params\n");
        }

        ifstream file(argv[1]);

        if (file.is_open()){
            getline(file, sequence);
            MPI_Bcast(&go, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
            file.close();
        }else{
            throwError(2, "Unable to open file\n");
        }
        segmentSize = floor(sequence.length()/numProcesses);
    } else
        MPI_Bcast(&go, 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    if(go == 'N')
        throw 1; // THROW

    return 0;
}


int main(int argc, char *argv[])
{
    int myRank, numProcesses;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);


    string sequence;
    //string sequenceProcess;

    int sequence_count[26];
    int sequence_count_global[26];

    initArray(sequence_count, 26);

    int segmentSize = 0;

    try {
        validateInputData(myRank, argc, argv, segmentSize, sequence, numProcesses);
    } catch (int error) {
        return 0;
    }


    MPI_Bcast(&segmentSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    char* sequenceBuffer = new char[segmentSize + 1];
    MPI_Scatter(sequence.c_str(), segmentSize, MPI_CHAR, sequenceBuffer, segmentSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    int index;
    #pragma omp parallel for default(shared) private(index) shared(sequenceBuffer, segmentSize) reduction(+:sequence_count)
    for (int i = 0; i < segmentSize; ++i)
    {
        index = sequenceBuffer[i] - 'a';
        if (index < 26 && index >= 0) {
            sequence_count[index]++;
        }
        else {
            std::cout << "Invalid char [" << sequenceBuffer[i] << "]" << std::endl;
        }
    }

    MPI_Reduce(sequence_count, sequence_count_global, 26, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myRank == 0){
        const int tmpSequenceLen = sequence.length();
        int leftHanging = tmpSequenceLen % numProcesses;
        for (int i = leftHanging; i > 0; --i)
        {
            index = sequence[tmpSequenceLen - i] - 'a';
            if (index < 26 && index >= 0)
            {
                sequence_count_global[index]++;
            }
            else
            {
                cout << "Invalid char [" << sequenceBuffer[i] << "]" << endl;
            }
        }


        cout << "Sequence occurrences:";

        for (int j = 0; j < 26; ++j)
        {
            if (sequence_count_global[j] > 0)
            {
                cout << " " << (char)('a' + j) << "=" << sequence_count_global[j];
            }
        }

        cout << endl;
    }
    delete [] sequenceBuffer;

    MPI_Finalize();

    return 0;
}
