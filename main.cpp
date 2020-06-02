#include <iostream>
#include <vector>
#include <fstream>
#include "Matrix.hpp"

constexpr int max(int f, int s) {
    return f > s ? f : s;
}

template<typename T>
class test{
public:
    test() = delete ;
};

int main() {

    std::ifstream first("first.txt");
    std::ifstream second("second.txt");

    //AbstractMatrix::set_multiplier();


    SquareMatrix<int> matrix1(2);
    matrix1.set_thread_cnt(2);
    matrix1.read(first);
    std::cout << '\n';
    //matrix1.set_multiplier(multiplier<int, Buffer<int>>)
    SquareMatrix<int> matrix2(2);
    matrix2.read(second);
    std::cout <<'\n';

   // SquareMatrix<int> matrix3(2);



    SquareMatrix<int> matrix3 = matrix1 * matrix2;

    matrix3.write(std::cout);
    return 0;
}
