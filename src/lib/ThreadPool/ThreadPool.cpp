#include "ThreadPool/ThreadPool.h"
#include <condition_variable>
#include <functional>
#include <mutex>

ThreadPool::ThreadPool(size_t maxThreads)
    : maxThreads(maxThreads), stop(false) {
  threadPool.reserve(maxThreads);
}

void ThreadPool::WorkerWrapper() {

  while (true) {

    std::function<void()> task;
    {
      // wait until queue is available
      std::unique_lock taskQueueLock(taskQueueMutex);

      // wait for access to task queue, check if there is an actual task inside
      taskQueueAccess.wait(taskQueueLock, [&]() { return !taskQueue.empty(); });

      if (stop && !taskQueue.empty())
        break;

      if (taskQueue.empty())
        continue;

      task = taskQueue.front();
      taskQueue.pop();
    }

    task();
  }
}

void ThreadPool::AddWorker() {
  if (threadPool.size() >= maxThreads) {
    throw ThreadPoolException(
        "Could not add another thread pool worker, limit reached");
  }

  threadPool.emplace_back(&ThreadPool::WorkerWrapper, this);
}

void ThreadPool::AddWorker(size_t threads) {
  for (size_t i = 0; i < threads; ++i) {
    AddWorker();
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(taskQueueMutex);
    stop = true;
  }

  taskQueueAccess.notify_all();
  for (auto &worker : threadPool) {
    worker.join();
  }
}
