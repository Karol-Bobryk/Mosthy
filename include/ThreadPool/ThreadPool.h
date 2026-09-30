#ifndef THREAD_POOL
#define THREAD_POOL

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPoolException : std::runtime_error {
public:
  explicit ThreadPoolException(
      const std::string &message = "Thread pool exception occured")
      : std::runtime_error(message) {}
};

class ThreadPool {
private:
  using WorkerFunction = std::function<void()>;
  std::vector<std::thread> threadPool;

  size_t maxThreads;

  std::queue<WorkerFunction> taskQueue;

  std::condition_variable taskQueueAccess;
  std::mutex taskQueueMutex;
  // after stop is signaled, task queue will be drained first
  bool stop;

  void WorkerWrapper();

public:
  ThreadPool(size_t maxThreads);
  ~ThreadPool();

  template <typename F, typename... Args>
  auto AddTask(F &&f, Args &&...args) -> std::future<decltype(f(args...))> {
    // TODO: this might get changed later, as for now you can't pre-cache tasks
    // before any workers exist
    if (threadPool.size() == 0)
      throw ThreadPoolException(
          "Could not assign a task without workers spawned");

    auto taskFunc = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    auto encapsulatedTask =
        std::make_shared<std::packaged_task<decltype(f(args...))()>>(taskFunc);

    std::future<std::invoke_result_t<F, Args...>> taskFuture =
        encapsulatedTask->get_future();
    {
      std::unique_lock<std::mutex> lock(taskQueueMutex);
      taskQueue.emplace([&encapsulatedTask]() { (*encapsulatedTask)(); });
    }

    taskQueueAccess.notify_one();

    return taskFuture;
  }

  void AddWorker();
  void AddWorker(size_t threads);
  void GetWorker(std::packaged_task<WorkerFunction> &&worker);
};

#endif
