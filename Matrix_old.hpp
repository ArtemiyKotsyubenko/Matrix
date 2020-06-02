//
// Created by artemiy on 02.06.2020.
//

#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <iostream>

template<unsigned N, unsigned M, typename T, class Container = std::vector<std::vector<T>>>
class Matrix {
private:

public:

    static unsigned threads_cnt;

    Matrix() = default;

    Matrix(const Matrix &other);

    Matrix(Matrix &&other) {

    }

    virtual ~Matrix() = default;

    Matrix &operator=(const Matrix &other);

    Matrix &operator=(Matrix &&other);


    Matrix &operator+=(const Matrix<N, M, T, Container> &other);

    Matrix &operator-=(const Matrix<N, M, T, Container> &other);


    template<unsigned A, unsigned B, typename U, typename Cont>
    friend Matrix<A, B, U, Cont> operator+(const Matrix<A, B, U, Cont> &first, const Matrix<A, B, U, Cont> &second);

    template<unsigned A, unsigned B, typename U, typename Cont>
    friend Matrix<A, B, U, Cont> operator-(const Matrix<A, B, U, Cont> &first, const Matrix<A, B, U, Cont> &second);

    template<unsigned A, unsigned B, unsigned C, typename U, typename Cont>
    friend Matrix<A, C, U, Cont> operator*(const Matrix<A, B, U, Cont> &first, const Matrix<B, C, U, Cont> &second);


};

template<unsigned N, typename T, class Container>
class SquareMatrix : public Matrix<N, N, T, Container> {

};

#endif //MATRIX_MATRIX_HPP
