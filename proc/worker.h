#pragma once

#include <deque>
#include <atomic>
#include <functional>

#include <chrono>
#include <random>
#include <math.h>

#include "framework/cds.h"
#include "framework/cache_isolated.h"

// Implementation based on the steal-one sender-initiated variant of 
// [Scheduling Parallel Programs by Work Stealing with Private Deques](http://www.chargueraud.org/research/2013/ppopp/full.pdf)
// by Acar, Charguéraud, and Rainey
namespace proc {

  struct pool;
  struct worker;

  using task = std::function<void(worker&)>;

  static const size_t max_workers = 16;
  static const double task_delta = 0.0002;

  struct worker {
    template <typename SeedSeq> worker(pool &p, int i, SeedSeq & seed) : rng(seed), q(std::move(q)), i(i), p(p) {}
    std::mt19937 rng;
    std::deque<task> q; // local jobs
    pool & p; // owning pool
    int i; // worker id
    void main();
  };

  struct pool {
    // in between the time we start and the time we stop tasks are running. 
    // This provides no mechanism to detect their state, however.
    template <typename ... Ts> pool(int N, std::mt19937 r, Ts && ... args); // give us a list of starting tasks
    virtual ~pool();
   
    int N;
    framework::cache_isolated<std::atomic<task*>> s[max_workers]; // messaging primitives
    std::vector<std::thread> threads;
    std::atomic<bool> shutdown;
    std::vector<worker> workers;

    void run(task) {} // enqueue a task

    template <typename ... T, typename F> void run(F && f, T && ... args) {
      run(std::function(std::forward(f), std::forward(args)...));
    }

    void run(int, task) {} // enqueue a task with affinity    

    template <typename ... T, typename F>
    void run(int i, F && f, T && ... args) {
      run(i, std::function(std::forward(f), std::forward(args)...));
    }
  };

/*
  // assumes we're a worker
  extern void defer(task) {}
  template <typename ... T, typename F>
  void defer(F && f, T && ... args) {
    defer(std::function(std::forward(f), std::forward(args)...));
  }

  extern void defer(int, task) {}

  template <typename ... T, typename F>
  void defer(int i, F && f, T && ... args) {
    defer(i, std::function(std::forward(f), std::forward(args)...));
  }

  //using fiber = std::function<void(worker&)>;
  // we could do this, but it needs to be more explicit so we can have members
  //template <typename T> using par = std::function<fiber(fiber(T))>; 
*/
  namespace detail {
    struct dummy_task : task {};
    extern dummy_task the_dummy_task;
  };

  template <typename ... Ts> pool::pool(int N, std::mt19937 rng, Ts && ... args) : N(N) {

    for (int i = 0;i < N;++i)
      s[i].data.store(&detail::the_dummy_task, memory_order_relaxed);

    shutdown.store(false, memory_order_relaxed);

    int j = 0;

    
    for (int i = 0;i < N;++i)
      workers.push_back(worker(*this, i, seed_seq{ rng(), rng(), rng(), rng() }));


    {
      int i = 0;
      // pre-load our starting tasks
      for (auto && task : { std::function<void(worker&)>(args) ... }) {
        workers[i++].q.push_front(task); // distribute tasks round-robin to start before the threads kick in
        i %= N;
      }
    }

    for (int i = 0; i < N;++i) {     
      worker & w = workers[i];
      threads.push_back(std::thread([&] { w.main(); }));
    }
  }

}