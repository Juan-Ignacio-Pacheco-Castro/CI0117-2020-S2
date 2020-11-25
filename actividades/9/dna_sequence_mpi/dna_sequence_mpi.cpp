#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "mpi/mpi.h"

using namespace std;

void initArray(int *array, int size)
{
    for (int i = 0; i < size; ++i)
    {
        array[i] = 0;
    }
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


    if (argc < 2)
    {
        cout << "Invalid number of params." << endl;
        return 1;
    }

    if (myRank == 0){
        ifstream file(argv[1]);

        if (file.is_open())
        {
            getline(file, sequence);
            file.close();
        }
        else
        {
            cout << "Unable to open file" << argv[1] << endl;
            return 1;
        }

        //std::cout << sequence << std::endl;
        segmentSize = floor(sequence.length()/numProcesses);

        /*if (sequence.length() % numProcesses != 0)
            segmentSize = (sequence.length() - (sequence.length() % numProcesses))/numProcesses;
        std::cout << segmentSize << std::endl;*/
    }


    //MPI_Scatter(&sequence, sequence.length()/numProcesses, MPI_CHAR, &sequenceProcess, sequence.length()/numProcesses, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&segmentSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    char* sequenceBuffer = new char[segmentSize + 1];
    MPI_Scatter(sequence.c_str(), segmentSize, MPI_CHAR, sequenceBuffer, segmentSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    //std::cout << "Segment size = " << segmentSize << std::endl;

    int index;
    for (int i = 0; i < segmentSize; ++i)
    {
        index = sequenceBuffer[i] - 'a';
        if (index < 26 && index >= 0)
        {
            sequence_count[index]++;
        }
        else
        {
            cout << "Invalid char [" << sequenceBuffer[i] << "]" << endl;
        }
    }
    //std::cout << "Sequence_count =" << sequence_count[1] << std::endl;

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
    delete sequenceBuffer;

    MPI_Finalize();

    return 0;
}
