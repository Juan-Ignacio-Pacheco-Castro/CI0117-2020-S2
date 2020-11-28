
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <omp.h>

int main(int argc, char *argv[]) {
    int n;

    if (argc > 1){
        n = std::stoi(argv[1]);
    } else {
        std::cerr << "Invalid number of params" << std::endl;
        return 0;
    }

    double factor = 1.0;
    double sum = 0.0;
    for (int k = 0; k < n; ++k){
        sum += factor/(2*k+1);
        factor = -factor;
    }

    double pi = 4.0 * sum;

    std::cout << "Valor de pi = " << pi << std::endl;

    return 0;
}
