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
#define MIN_THREADS 4

namespace sockserver {
class ThreadPool {
  std::vector<std::thread> pool;
  std::queue<std::function<void(const std::condition_variable &, const std::atomic_bool &)>>
      tasks;
  std::vector<std::thread::id> idle_workers;
  std::vector<std::thread::id> workers_to_kill;
  unsigned int min_threads;
  std::mutex mut;
  std::atomic_bool stop;
  std::condition_variable cond;
  std::atomic_uint task_count;
  std::atomic_uint num_running_threads;
  std::atomic_uint num_threads;
  void init_pool();
  void spawn_thread();
 public:
  ThreadPool();
  ~ThreadPool();
  void schedule(const std::function<void(const std::condition_variable &, const std::atomic_bool &)> &task);
};
}