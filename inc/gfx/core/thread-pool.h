#pragma once

#include <vector>
#include <atomic>
#include <thread>
#include <functional>
#include <barrier>

namespace gfx
{

class ThreadPool
{

public:

    explicit ThreadPool(int num_threads);

    ~ThreadPool();

    template<typename Fn>
    void run(const int count, Fn&& fn)
    {
        work_fn = std::forward<Fn>(fn);
        total_work = count;
        next_index.store(0, std::memory_order_relaxed);

        barrier.arrive_and_wait();

        barrier.arrive_and_wait();
    }

private:

    void worker_loop();

    std::vector<std::thread> workers;
    std::barrier<> barrier;

    std::atomic<bool> running;
    std::atomic<int> next_index { 0 };
    int total_work { 0 };

    std::function<void(int)> work_fn;};

}
