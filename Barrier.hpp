//
// Created by artemiy on 03.06.2020.
//

#ifndef MATRIX_BARRIER_HPP
#define MATRIX_BARRIER_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>

class Barrier {//class for global synchronization
private:

    const u_int threads_count;
    std::atomic<u_int> threads_waiting;
    std::condition_variable cv;
    std::mutex mut;
    std::function<void()> behaviour_before_waking_up = [](){};

public:

    Barrier(const int &threads_count_) :
            threads_count(threads_count_),
            threads_waiting(0) {}


    Barrier(const Barrier &barrier) = delete;

    Barrier &operator=(const Barrier &barrier) = delete;

    ~Barrier() = default;

    void set_behaviour_before_waking_up(std::function<void()> &&func) {
        behaviour_before_waking_up = func;
    }// void function to execute when all threads reached barrier
    void set_behaviour_before_waking_up(std::function<void()> func) {
        behaviour_before_waking_up = func;
    }

    void wait() {// call to fall asleep
        if (threads_waiting.fetch_add(1) >= threads_count - 1) {
            threads_waiting.store(0);
            behaviour_before_waking_up();
            cv.notify_all();
        } else {
            std::unique_lock<std::mutex> lock(mut);
            cv.wait(lock);
        }
    }
};
// wasn't used in SquareMatrix


#endif //MATRIX_BARRIER_HPP
