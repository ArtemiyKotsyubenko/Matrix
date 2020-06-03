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

    static  size_t thread_cnt_;

    virtual void multiplier(const AbstractMatrix<T, Container> &first, const AbstractMatrix<T, Container> &second) = 0;

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
                                                  matrix_(other.matrix_) {}

    AbstractMatrix(AbstractMatrix &&other) : height_(other.height_),
                                             width_(other.width_),
                                             matrix_(other.matrix_) {}

    virtual ~AbstractMatrix() = default;

    AbstractMatrix &operator=(AbstractMatrix &&other);

    virtual AbstractMatrix &operator=(const AbstractMatrix &other);

    bool operator ==(const AbstractMatrix& other){
        for(size_t i = 0; i < height_; ++i){
            for(size_t j = 0; j < width_; ++j){
                if(matrix_[i][j] != other.matrix_[i][j]){
                    std::cout << other.matrix_[i][j] << std::endl;
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
    friend typename std::enable_if_t<std::is_base_of<AbstractMatrix<U, Cont>, DerivedMatrix<U, Cont>>::value, DerivedMatrix<U, Cont>>
    operator*(const DerivedMatrix<U, Cont> &first, const DerivedMatrix<U, Cont> &second);

public:


};

template<typename T, class Container> size_t AbstractMatrix<T, Container>::thread_cnt_ = 4;

template<template<typename V, class C> class DerivedMatrix, typename U, class Cont>
std::enable_if_t<std::is_base_of<AbstractMatrix<U, Cont>, DerivedMatrix<U, Cont>>::value, DerivedMatrix<U, Cont>>
operator*(const DerivedMatrix<U, Cont> &first, const DerivedMatrix<U, Cont> &second) {

    if (first.width() != second.height()) {
        throw std::invalid_argument("Invalid matrix size");
    }

    DerivedMatrix<U, Cont> new_matrix(first.width(), second.height());
    AbstractMatrix<U, Cont> &ref = new_matrix;
    ref.multiplier(first, second);
    return new_matrix;
}

template<typename T, class Container>
template<typename StreamClass>
void AbstractMatrix<T, Container>::read(StreamClass &strm) {
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            int temp;
            strm >> temp;
            //matrix_[i][j] = static_cast<std::atomic<int>>(temp);
            matrix_[i][j]= temp;
            //strm >> matrix_[i][j];
        }
    }
}

template<typename T, class Container>
template<typename StreamClass>
void AbstractMatrix<T, Container>::write(StreamClass &strm) {
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            strm << matrix_[i][j] << ' ';
        }
    }
}


template<typename T, class Container>
AbstractMatrix<T, Container> &AbstractMatrix<T, Container>::operator=(AbstractMatrix &&other) {
    if (height_ != other.height_ || width_ != other.width_) {
        throw std::invalid_argument("incorrect size");
    }
    matrix_ = other.matrix_;
    return *this;
}

template<typename T, class Container>
AbstractMatrix<T, Container> &AbstractMatrix<T, Container>::operator=(const AbstractMatrix &other) {
    if (height_ != other.height_ || width_ != other.width_) {
        throw std::invalid_argument("incorrect size");
    }
    matrix_ = other.matrix_;
    return *this;
}

//*********************************************************************************************************************

template<typename T, typename Container = Buffer<T>>
class SquareMatrix : public AbstractMatrix<T, Container> {
private:

    void multiplier(const AbstractMatrix<T, Container> &first,
                    const AbstractMatrix<T, Container> &second) override {
        auto &first_ = reinterpret_cast<const SquareMatrix &>(first);
        auto &second_ = reinterpret_cast<const SquareMatrix &>(second);


        size_t step = first.height() / AbstractMatrix<T, Container>::get_thread_cnt();

        if(step == 0){
            step = first.height();
        }

        std::atomic_long current(0);
        //size_t current = 0;
        std::atomic_long max(first.height()* second.width());

        std::mutex mut;
        int size = 0;
        std::vector<int> vec(100);

        Barrier barrier(AbstractMatrix<T, Container>::get_thread_cnt());
        auto worker = [&](const size_t left_border, const size_t right_border) {

            //std::vector<std::pair<int, int>>v;
            while (current < max){
                //mut.lock();
                size_t copy = current.fetch_add(1);
                //mut.unlock();
                //size_t  copy = current++;
//                if(copy >= max){
//                    break;
//                }
                size_t x = copy % 100;
                size_t y = copy / 100;
                //++vec[x];
                //v.emplace_back(x, y);
                //std::cout <<

                AbstractMatrix<T, Container>::matrix_ [y][x] = 0;
                for(size_t i = 0; i < first.height(); ++i){
                    //mut.lock();
                    AbstractMatrix<T, Container>::matrix_ [y][x]+= first_.matrix_[y][i] * second_.matrix_[i][x];
                    //mut.unlock();
                }
            }


//            mut.lock();
//            for(auto pair: v){
//                std::cout << pair.first << ' '<< pair.second<< '\n';
//            }
//            size+= v.size();
//            mut.unlock();

//            for (int i = left_border; i < right_border; ++i) {
//                for (int j = 0; j < second.height(); ++j) {
//                    AbstractMatrix<T, Container>::matrix_[i][j] = 0;
//                    for (int k = 0; k < first.width(); ++k) {
//                        auto  temp = first_.matrix_[i][k] * second_.matrix_[k][j];
//                        AbstractMatrix<T, Container>::matrix_[i][j].fetch_add(temp);
//                    }
//                }
//            }

            //barrier.wait();
        };

        std::vector<std::thread> workers;
        size_t left_border = 0;

        for (size_t thread_num = 1; thread_num < AbstractMatrix<T, Container>::get_thread_cnt(); ++thread_num) {
            workers.emplace_back(worker, left_border, left_border + step);
            left_border += step;
        }

        worker(left_border, first.height());
        //workers.emplace_back(worker, left_border, first.height());

        for (auto &worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
//        for(auto  it : vec){
//            std::cout << it << ' ';
//        }
//        std::cout << std::endl;

        //std::cout << '\n'<< size<< '\n';

    }

protected:

public:
    explicit SquareMatrix(const size_t size) : AbstractMatrix<T, Container>(size, size) {};

    explicit SquareMatrix(const size_t size, const size_t size1) : AbstractMatrix<T, Container>(size, size) {};

    ~SquareMatrix() = default;
};

#endif //MATRIX_MATRIX_HPP
