//
// Created by tobias on 27/10/16.
//

#include <algorithm>
#include <utility>
#include <iostream>
#include "sockserver/ThreadPool.h"

namespace sockserver {

ThreadPool::ThreadPool() : stop(false), task_count(0), num_threads(0), num_running_threads(0) {
  this->min_threads =
      std::thread::hardware_concurrency() < MIN_THREADS ? MIN_THREADS
                                                        : std::thread::hardware_concurrency();
  this->init_pool();
}

ThreadPool::~ThreadPool() {
  this->stop = true;
  this->cond.notify_all();
  for (auto &t : this->pool) {
    t.join();
  }

}

void ThreadPool::init_pool() {
  for (unsigned int i = 0; i < this->min_threads; ++i) {
    this->spawn_thread();
  }
}

void ThreadPool::schedule(const std::function<void(const std::atomic_bool &)> &task) {

  ++this->task_count;

  /* there are not enough threads, lets spawn a bunch of them */
  if (this->num_threads < this->task_count) {
    std::cout << this->num_running_threads << " - " << this->task_count << std::endl;
    for (int i = 0; i < this->min_threads; ++i) {
      this->spawn_thread();
    }
  }
  {
    std::unique_lock<std::mutex> lock(this->mut);
    this->tasks.push(task);
  }
  this->cond.notify_one();
  if ((this->num_threads - this->num_running_threads) > 2 * this->min_threads) {
    {
      std::unique_lock<std::mutex> lock(this->mut);
      this->workers_to_kill = std::vector<std::thread::id>(this->idle_workers.begin(),
                                                           this->idle_workers.begin() + this->min_threads);
    }
    this->cond.notify_all();
  }
}

void ThreadPool::spawn_thread() {
  this->pool.emplace_back([this]() {
    auto id = std::this_thread::get_id();
    {
      std::unique_lock<std::mutex> lock(this->mut);
      this->idle_workers.push_back(id);
    }
    while (true) {
      std::function<void(const std::atomic_bool &)> task;
      {
        std::unique_lock<std::mutex> lock(this->mut);
        this->cond.wait(lock, [this, id]() -> bool {
          return (
              !this->tasks.empty()
                  || this->stop
                  || (!this->workers_to_kill.empty()
                      && (std::find(this->workers_to_kill.begin(), this->workers_to_kill.end(), id)
                          != this->workers_to_kill.end())
                  )
          );
        });

        if (this->stop && this->tasks.empty()) {
          --num_threads;
          return;
        } else if (std::find(this->workers_to_kill.begin(), this->workers_to_kill.end(), id)
            != this->workers_to_kill.end()) {
          this->workers_to_kill.erase(
              std::remove(this->workers_to_kill.begin(), this->workers_to_kill.end(), id),
              this->workers_to_kill.end()
          );
          this->idle_workers.erase(
              std::remove(this->idle_workers.begin(), this->idle_workers.end(), id),
              this->idle_workers.end()
          );
          --num_threads;
          return;
        }

        task = std::move(this->tasks.front());
        this->tasks.pop();
        this->idle_workers.erase(
            std::remove(this->idle_workers.begin(), this->idle_workers.end(), id),
            this->idle_workers.end()
        );
        ++this->num_running_threads;
      }
      task(this->stop);
      {
        std::unique_lock<std::mutex> lock(this->mut);
        --this->num_running_threads;
        --this->task_count;
        this->idle_workers.push_back(id);
      }
    }
  });
  ++this->num_threads;
}
}