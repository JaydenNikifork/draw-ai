#ifndef POOL_H
#define POOL_H

#include <thread>
#include <future>
#include <unordered_set>
#include <queue>
#include <functional>

class ThreadPool {
  unsigned int cap;
  std::unordered_set<std::thread> workers;

  ThreadPool(unsigned int cap): cap{cap} {}



public:
  template<typename Fn, typename... Args>
  const std::thread &doJob(Fn &&fn, Args&&... args) {
    
  }
};

#endif