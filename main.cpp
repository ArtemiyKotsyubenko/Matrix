#include <iostream>
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
    std::ifstream ans("ans.txt");
    std::ofstream output("output.txt");

    //AbstractMatrix::set_multiplier();

    SquareMatrix<std::atomic<int>> matrix1(100);
    matrix1.set_thread_cnt(2);
    matrix1.read(first);
    //std::cout << '\n';
    //matrix1.set_multiplier(multiplier<int, Buffer<int>>)
    SquareMatrix<std::atomic<int>> matrix2(100);
    matrix2.read(second);

    SquareMatrix<std::atomic<int>> res(100);
    res.read(ans);
    //std::cout <<'\n';

   // SquareMatrix<int> matrix3(2);




    SquareMatrix<std::atomic<int>> matrix3 = matrix1 * matrix2;
    std::cout << (res == matrix3)<< std::endl;


    matrix3.write(std::cout);
    std::cout << std::endl;
    res.write(std::cout);
    return 0;
}
