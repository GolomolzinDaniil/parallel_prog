#include <iostream>
#include <cmath>


using arr_type =
    #ifdef DOUBLE
        double
    #else
        float
    #endif
    ;

const size_t ARR_SIZE = 10'000'000;
arr_type* ARR = new arr_type[ARR_SIZE];

int main() {

    arr_type step = 2 * M_PI / ARR_SIZE;
    
    arr_type sum = 0.0;
    for (size_t i = 0; i < ARR_SIZE; ++i) {
        ARR[i] = std::sin(step * i);
        sum += ARR[i];
    }
    delete[] ARR;

    std::cout << "arr_type = "
              << (sizeof(arr_type) == sizeof(float) ? "float" : "double")
              << ", sum = " << sum << std::endl;
    return 0;
}