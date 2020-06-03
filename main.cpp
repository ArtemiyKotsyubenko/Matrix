#include <iostream>
#include <fstream>
#include "Matrix.hpp"

template<typename T>
class test{
public:
    test() = delete ;
};

int main() {

    std::ifstream first("first.txt");
    std::ifstream second("second.txt");
    std::ifstream ans("ans.txt");
    std::ofstream output("output.txt");

    SquareMatrix<unsigned > matrix1(100);
    matrix1.set_thread_cnt(8);
    matrix1.read(first);
    SquareMatrix<unsigned > matrix2(100);
    matrix2.read(second);

    SquareMatrix<unsigned > res(100);
    res.read(ans);

    SquareMatrix<unsigned > matrix3 = matrix1 * matrix2;
    std::cout << (res == matrix3)<< std::endl;


    matrix3.write(output);
    //std::cout << std::endl;
    //res.write(std::cout);
    return 0;
}
