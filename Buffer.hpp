//
// Created by artemiy on 02.06.2020.
//

#ifndef MATRIX_BUFFER_HPP
#define MATRIX_BUFFER_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>

template<typename T>
class Buffer {// cash-friendly 2D array
private:

    T *array_begin_ = nullptr;

    const size_t height_;
    const size_t width_;

    struct proxy {// class helper for maintaining usual access by [][]
    private:
        T *ptr_ = nullptr;
    public:

        proxy(T *ptr) : ptr_(ptr) {}

        T &operator[](const size_t index) {
            return *(ptr_ + index);
        }

        const T &operator[](const size_t index) const {
            return *(ptr_ + index);
        }
    };

public:

    size_t height() const noexcept { return height_; }

    size_t width() const noexcept { return width_; }

    Buffer(const size_t height, const size_t width) : height_(height), width_(width) {
        array_begin_ = new T[height_ * width_];
    }

    Buffer(const Buffer &other);

    Buffer(Buffer &&other) noexcept;

    Buffer &operator=(const Buffer &other);

    Buffer &operator=(Buffer &&other);

    ~Buffer() {
        delete[] array_begin_;
    }


    proxy operator[](size_t index);

    const proxy operator[](size_t index) const;
};

template<typename T>
typename Buffer<T>::proxy Buffer<T>::operator[](size_t index) {
    proxy proxy_(array_begin_ + width_ * index);
    return proxy_;
}

template<typename T>
const typename Buffer<T>::proxy Buffer<T>::operator[](size_t index) const {
    proxy proxy_(array_begin_ + width_ * index);
    return proxy_;
}

template<typename T>
Buffer<T>::Buffer(const Buffer &other): height_(other.height_), width_(other.width_) {
    size_t size = height_ * width_;
    array_begin_ = new T[size];
    for (size_t i = 0; i < size; ++i) {
        array_begin_[i] = other.array_begin_[i];
    }
}

template<typename T>
Buffer<T>::Buffer(Buffer &&other) noexcept : height_(other.height_), width_(other.width_) {
    array_begin_ = other.array_begin_;
    other.array_begin_ = nullptr;
}

template<typename T>
Buffer<T> &Buffer<T>::operator=(Buffer &&other) {
    if (height_ != other.height_ || width_ != other.width_) {
        throw std::invalid_argument("incorrect size");
    }
    delete[] array_begin_;
    array_begin_ = other.array_begin_;
    other.array_begin_ = nullptr;
    return *this;
}

template<typename T>
Buffer<T> &Buffer<T>::operator=(const Buffer &other) {
    if (height_ != other.height_ || width_ != other.width_) {
        throw std::invalid_argument("incorrect size");
    }
    size_t size = height_ * width_;
    for (size_t i = 0; i < size; ++i) {
        array_begin_[i] = other.array_begin_[i];
    }
    return *this;
}

#endif //MATRIX_BUFFER_HPP
