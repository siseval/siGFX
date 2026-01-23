#include <gfx/core/thread-pool.h>

#include <vector>
#include <atomic>
#include <thread>
#include <functional>

namespace gfx
{

ThreadPool::ThreadPool(int num_threads)
    : barrier(num_threads + 1),
      running(true)
{
    for (int i = 0; i < num_threads; ++i)
    {
        workers.emplace_back([this] { worker_loop(); });
    }
}

ThreadPool::~ThreadPool()
{
    running = false;
    barrier.arrive_and_drop();
    for (auto &worker : workers)
    {
        if (worker.joinable())
        {
            worker.join();
        }
    }
}

void ThreadPool::worker_loop()
{
    while (running)
    {
        barrier.arrive_and_wait();

        if (!running)
        {
            return;
        }

        while (true)
        {
            int i = next_index.fetch_add(1, std::memory_order_relaxed);
            if (i >= total_work)
            {
                break;
            }

            work_fn(i);
        }

        barrier.arrive_and_wait();
    }
}


}
