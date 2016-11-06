//
// Created by tobias on 27/10/16.
//

#pragma once
#include <memory>
#include <map>
#include <thread>
#include <queue>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>

namespace sockserver {
/**
 * @class ThreadPool ThreadPool.h "sockserver/ThreadPool.h"
 * @brief A dynamic thread pool that can receive tasks as lambdas.
 */
class ThreadPool {
  /**
   * @var pool is the vector of threads
   */
  std::vector<std::thread> pool;
  /**
   * @var tasks is a queue of functions
   */
  std::queue<std::function<void(const std::atomic_bool &)>>
      tasks;
  /**
   * @var idle_workers hold the ids of all threads that aren't running a task
   */
  std::vector<std::thread::id> idle_workers;
  /**
   * @var workers_to_kill holds the ids of threads that should stop
   */
  std::vector<std::thread::id> workers_to_kill;
  /**
   * @var min_threads is the initial number of threads and the number of threads to grow
   */
  unsigned int min_threads;
  /**
   * @var mut is the mutex that is used to sync the threads
   */
  std::mutex mut;
  /**
   * @var stop is set to true if the threads should stop
   */
  std::atomic_bool stop;
  /**
   * @var cond is used to communicate with the threads
   */
  std::condition_variable cond;
  /**
   * @var task_count is the number of tasks that are currently executed/queued
   */
  std::atomic_uint task_count;
  /**
   * @var num_running_threads is the number of threads that are executing a task
   */
  std::atomic_uint num_running_threads;
  /**
   * @var num_threads is the number of existing threads
   */
  std::atomic_uint num_threads;
  void init_pool();
  void spawn_thread();
 public:
  ThreadPool(unsigned int min_threads);
  ~ThreadPool();
  void schedule(const std::function<void(const std::atomic_bool &)> &task);
};
}