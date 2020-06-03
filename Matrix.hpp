//
// Created by artemiy on 02.06.2020.
//

#include "Buffer.hpp"
#include "Barrier.hpp"
#include <vector>
#include <exception>
#include <stdexcept>

#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP
//
//template<typename T, class Container>
//class Matrix {
//private:
//    static size_t thread_cnt_;
//
//    const size_t height_;
//    const size_t width_;
//
//
//protected:
//    Container matrix_;
//
//public:
//
//
//    template<typename StreamClass>
//    void read(StreamClass &strm);
//
//    template<typename StreamClass>
//    void write(StreamClass &strm);
//
//    Matrix(const size_t width, const size_t height) : height_(height), width_(width), matrix_(height, width) {}
//
//    Matrix(const Matrix &other);
//
//    Matrix(Matrix &&other);
//
//    virtual ~Matrix() = default;
//
//    Matrix &operator=(Matrix &&other);
//
//    virtual Matrix &operator=(const Matrix &other);
//
//
//    size_t height() const { return height_; }
//
//    size_t width() const { return width_; }
//
//    static void set_thread_cnt(const size_t thread_cnt) noexcept { thread_cnt_ = thread_cnt; }
//
//    static size_t get_thread_cnt() noexcept { return thread_cnt_; }
//
//};
//
//template<typename T, class Container>
//Matrix<T, Container>::Matrix(const Matrix &other):height_(other.height_), width_(other.width_),
//                                                  matrix_(other.matrix_) {}
//
//template<typename T, class Container>
//Matrix<T, Container>::Matrix(Matrix &&other): height_(other.height_), width_(other.width_), matrix_(other.matrix_) {}
//
//template<typename T, class Container>
//Matrix<T, Container> &Matrix<T, Container>::operator=(Matrix &&other) {
//    if (height_ != other.height_ || width_ != other.width_) {
//        throw std::invalid_argument("incorrect size");
//    }
//    matrix_ = other.matrix_;
//    return *this;
//}
//
//
//template<typename T, class Container>
//Matrix<T, Container> &Matrix<T, Container>::operator=(const Matrix &other) {
//    if (height_ != other.height_ || width_ != other.width_) {
//        throw std::invalid_argument("incorrect size");
//    }
//    matrix_ = other.matrix_;
//    return *this;
//}


template<typename T, class Container = Buffer<T> >
class AbstractMatrix {
private:

    static size_t thread_cnt_;

    virtual void multiplier(const AbstractMatrix<T, Container> &first, const AbstractMatrix<T, Container> &second) = 0;

    /*We definitely sure that friend operator*  for basic class such has template that allows to use it only derived
     * classes.
     * to make it work do reinterpret_cast from AbstractMatrix to your class*/

    const size_t height_;
    const size_t width_;

protected:
    Container matrix_;

public:

    template<typename StreamClass>
    void read(StreamClass &strm);

    template<typename StreamClass>
    void write(StreamClass &strm);

    AbstractMatrix(const size_t width, const size_t height) : height_(height),
                                                              width_(width),
                                                              matrix_(height, width) {}

    AbstractMatrix(const AbstractMatrix &other) : height_(other.height_),
                                                  width_(other.width_),
                                                  matrix_(other.matrix_) {}// copy constructor

    AbstractMatrix(AbstractMatrix &&other) : height_(other.height_),
                                             width_(other.width_),
                                             matrix_(other.matrix_) {}// move constructor

    virtual ~AbstractMatrix() = default;// destructor is virtual for correct derived classes' work

    AbstractMatrix &operator=(AbstractMatrix &&other);

    virtual AbstractMatrix &operator=(const AbstractMatrix &other);

    bool operator==(const AbstractMatrix &other) {
        for (size_t i = 0; i < height_; ++i) {
            for (size_t j = 0; j < width_; ++j) {
                if (matrix_[i][j] != other.matrix_[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }


    size_t height() const { return height_; }

    size_t width() const { return width_; }

    static void set_thread_cnt(const size_t thread_cnt) noexcept { thread_cnt_ = thread_cnt; }

    static size_t get_thread_cnt() noexcept { return thread_cnt_; }


    template<template<typename V, class C> class DerivedMatrix, typename U, class Cont>
    friend
    const typename std::enable_if_t
    <std::is_base_of<AbstractMatrix<U, Cont>, DerivedMatrix<U, Cont>>::value,
    DerivedMatrix<U, Cont>>
    operator*(const DerivedMatrix<U, Cont> &first, const DerivedMatrix<U, Cont> &second);
};

template<typename T, class Container>
size_t AbstractMatrix<T, Container>::thread_cnt_ = std::thread::hardware_concurrency();// set default optimal value


template<template<typename V, class C> class DerivedMatrix, typename U, class Cont>
// friend operator* for Basic class.(and all derived, so we needn't define  friend operators for every basic class)
const std::enable_if_t<std::is_base_of<AbstractMatrix<U, Cont>, DerivedMatrix<U, Cont>>::value, DerivedMatrix<U, Cont>>
operator*(const DerivedMatrix<U, Cont> &first, const DerivedMatrix<U, Cont> &second) {

    if (first.width() != second.height()) {// check if size is correct
        throw std::invalid_argument("Invalid matrix size");
    }

    DerivedMatrix<U, Cont> new_matrix(first.width(), second.height());
    AbstractMatrix<U, Cont> &ref = new_matrix;
    ref.multiplier(first, second);// call to overloaded private function multiplier
    return new_matrix;
    /*will work with any derived class*/
}

template<typename T, class Container>
template<typename StreamClass>
void AbstractMatrix<T, Container>::read(StreamClass &strm) {// awaits class that has overloaded operator >>
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            //int temp;
            strm >> matrix_[i][j];
        }
    }
    // it's possible to check if it's stream operator later
}

template<typename T, class Container>
template<typename StreamClass>
void AbstractMatrix<T, Container>::write(StreamClass &strm) {// awaits class that has overloaded operator >>
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            strm << matrix_[i][j] << ' ';
        }
    }
    // it's possible to check if it's stream operator later
}


template<typename T, class Container>
AbstractMatrix<T, Container> &AbstractMatrix<T, Container>::operator=(AbstractMatrix &&other) {
    if (height_ != other.height_ || width_ != other.width_) {
        throw std::invalid_argument("incorrect size");
    }
    matrix_ = other.matrix_;
    return *this;
}// move-assignment operator

template<typename T, class Container>
AbstractMatrix<T, Container> &AbstractMatrix<T, Container>::operator=(const AbstractMatrix &other) {
    if (height_ != other.height_ || width_ != other.width_) {
        throw std::invalid_argument("incorrect size");
    }
    matrix_ = other.matrix_;
    return *this;
}//copy- assignment operator

//*********************************************************************************************************************

template<typename T, typename Container = Buffer<T>>
class SquareMatrix : public AbstractMatrix<T, Container> {
private:

    void multiplier(const AbstractMatrix<T, Container> &first,
                    const AbstractMatrix<T, Container> &second) override {
        auto &first_ = reinterpret_cast<const SquareMatrix &>(first);
        auto &second_ = reinterpret_cast<const SquareMatrix &>(second);

        std::atomic_long current(0);// get guarantee that every cell will be counted only one time by the only thread
        size_t max(first.height() * second.width());// max value for cursor

        std::vector<int> vec(100);

        auto worker = [&]() {// thread func

            while (true) {
                size_t copy = current.fetch_add(1);
                if (copy >= max) {
                    break;
                }
                //count cell's coordinates
                size_t x = copy % first.height();
                size_t y = copy / second.width();

                AbstractMatrix<T, Container>::matrix_[y][x] = 0;// calculation
                for (size_t i = 0; i < first.height(); ++i) {
                    AbstractMatrix<T, Container>::matrix_[y][x] += first_.matrix_[y][i] * second_.matrix_[i][x];
                }
            }
        };

        std::vector<std::thread> workers;//thread pool

        for (size_t thread_num = 1; thread_num < AbstractMatrix<T, Container>::get_thread_cnt(); ++thread_num) {
            workers.emplace_back(worker);//deploying
        }

        worker();//deploy last thread;

        for (auto &worker : workers) {// wait for joining to avoid mistakes
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

protected:

public:

    SquareMatrix &operator=(const SquareMatrix &other);

    SquareMatrix &operator=(SquareMatrix &&other);

    explicit SquareMatrix(const size_t size) : AbstractMatrix<T, Container>(size, size) {}

    SquareMatrix(const size_t size, const size_t size1) : AbstractMatrix<T, Container>(size, size) {}

    SquareMatrix(const SquareMatrix &other) : AbstractMatrix<T, Container>(other) {}

    SquareMatrix(SquareMatrix &&other) : AbstractMatrix<T, Container>(other) {}

    ~SquareMatrix() = default;
};

template<typename T, typename Container>
SquareMatrix<T, Container> &SquareMatrix<T, Container>::operator=(const SquareMatrix &other) {
    AbstractMatrix<T, Container>::operator=(other);
    return *this;
}

template<typename T, typename Container>
SquareMatrix<T, Container> &SquareMatrix<T, Container>::operator=(SquareMatrix &&other) {
    AbstractMatrix<T, Container>::operator=(other);
    return *this;
}

#endif //MATRIX_MATRIX_HPP
