//
// Created by tobias on 27/10/16.
//

#include <algorithm>
#include <utility>
#include <iostream>
#include "sockserver/ThreadPool.h"

namespace sockserver {
/**
 * @brief Constructor that creates the inital pool and sets the member variables
 * @param min_threads is the minimum number of threads that should be spawned
 * @return void
 */
ThreadPool::ThreadPool(unsigned int min_threads) : stop(false), task_count(0), num_threads(0), num_running_threads(0) {
  this->min_threads =
      std::thread::hardware_concurrency() < min_threads ? min_threads
                                                        : std::thread::hardware_concurrency();
  this->init_pool();
}

/**
 * @brief Destructor that tries to stop and join all threads
 */
ThreadPool::~ThreadPool() {
  this->stop = true;
  this->cond.notify_all();
  for (auto &t : this->pool) {
    t.join();
  }
}

/**
 * @brief Initialise the pool and fill it with a minimum number of threads
 */
void ThreadPool::init_pool() {
  for (unsigned int i = 0; i < this->min_threads; ++i) {
    this->spawn_thread();
  }
}

/**
 * @brief Schedule a task. Can grow or shrink the pool.
 * @param task is a function that takes a bool that is used to signal the destructor call
 */
void ThreadPool::schedule(const std::function<void(const std::atomic_bool &)> &task) {
  /* a new task is added */
  ++this->task_count;
  /* there are not enough threads, lets spawn a bunch of them */
  if (this->num_threads < this->task_count) {
    for (int i = 0; i < this->min_threads; ++i) {
      this->spawn_thread();
    }
  }
  {
    /* add the task to the queue */
    std::lock_guard<std::mutex> lock(this->mut);
    this->tasks.push(task);
  }
  /* notify a thread to start working */
  this->cond.notify_one();
  /* check if the pool has more than double of min_threads idle threads */
  if ((this->num_threads - this->num_running_threads) > 2 * this->min_threads) {
    {
      std::lock_guard<std::mutex> lock(this->mut);
      /* shrink the pool by min_threads */
      this->workers_to_kill = std::vector<std::thread::id>(this->idle_workers.begin(),
                                                           this->idle_workers.begin() + this->min_threads);
    }
    /* tell the idle workers to check if they should stop */
    this->cond.notify_all();
  }
}

/**
 * @brief Spawn a new thread in the pool
 */
void ThreadPool::spawn_thread() {
  /* add the thread in place */
  /* Closures ftw! */
  this->pool.emplace_back([this]() {
    /* get the id of the thread */
    auto id = std::this_thread::get_id();
    {
      std::lock_guard<std::mutex> lock(this->mut);
      /* add the thread to the idle workers (waiting room) */
      this->idle_workers.push_back(id);
    }
    /* loop that is only ended if the thread is notified stop */
    while (true) {
      /* will hold the task from the queue */
      std::function<void(const std::atomic_bool &)> task;
      {
        std::unique_lock<std::mutex> lock(this->mut);
        /* wait until a condition is met */
        this->cond.wait(lock, [this, id]() -> bool {
          /* return false if waiting should be continued */
          /* stop wait if task queue is not empty */
          /* stop wait if stop is set to true */
          /* stop wait if id is in workers_to_kill vector */
          return (
              !this->tasks.empty()
                  || this->stop
                  || (!this->workers_to_kill.empty()
                      && (std::find(this->workers_to_kill.begin(), this->workers_to_kill.end(), id)
                          != this->workers_to_kill.end())
                  )
          );
        });
        /* return and decrease number of threads if stop is set */
        if (this->stop) {
          --num_threads;
          return;
        } else if (std::find(this->workers_to_kill.begin(), this->workers_to_kill.end(), id)
            != this->workers_to_kill.end()) {
          /* erase own id from workers_to_kill vector */
          this->workers_to_kill.erase(
              std::remove(this->workers_to_kill.begin(), this->workers_to_kill.end(), id),
              this->workers_to_kill.end()
          );
          /* erase own id from idle_workers vector */
          this->idle_workers.erase(
              std::remove(this->idle_workers.begin(), this->idle_workers.end(), id),
              this->idle_workers.end()
          );
          /* decrease number of threads */
          --num_threads;
          /* return */
          return;
        }
        /* get a task from the queue */
        task = std::move(this->tasks.front());
        this->tasks.pop();
        /* erase own id from idle workers vector */
        this->idle_workers.erase(
            std::remove(this->idle_workers.begin(), this->idle_workers.end(), id),
            this->idle_workers.end()
        );
        /* increase the number of running threads */
        ++this->num_running_threads;
      }
      /* start the task with the stop variable as param */
      task(this->stop);
      {
        /* task is finished */
        std::lock_guard<std::mutex> lock(this->mut);
        --this->num_running_threads;
        --this->task_count;
        /* add own id to idle workers */
        this->idle_workers.push_back(id);
      }
    }
  });
  /* one more worker */
  ++this->num_threads;
}
}