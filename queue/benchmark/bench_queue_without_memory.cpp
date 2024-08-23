#define DISABLE_MEMORY_OPT 1
#include "queue.hpp"
#include <benchmark/benchmark.h>

#include <boost/atomic.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>
#include <thread>
// ################################################################################
//  custom queue
// ################################################################################
/// @brief консьюмер для тестирования много поточной очереди
/// @tparam T тип хранимых двнных
/// @param q сама очерень
/// @param cfg конфигурация для теста
template <typename T>
void consume(ex_queue::equeue<T> &q, ex_queue::config cfg,
             std::vector<int> &consumed, std::atomic<int> &n_task) {
  T data;
  while (true) {
    int n = n_task.load();
    if (n < 0 && q.is_empty()) {
      break;
    }
    bool pass = q.pop(data);
    if (pass) {
      std::this_thread::sleep_for(cfg.c_time);
    }
  }
}
/// @brief  продьюсер для помещения данных в очередь
/// @tparam T данные
/// @param q очередь
/// @param cfg  конфигурация
template <typename T>
void produce(ex_queue::equeue<T> &q, ex_queue::config cfg, T data,
             std::atomic<int> &n_task) {
  while (true) {
    int n = n_task.load();
    if (n < 0) {
      break;
    }
    while (!n_task.compare_exchange_weak(n, n - 1, std::memory_order_relaxed)) {
      if (n < 0)
        return;
      std::this_thread::yield();
    }
    std::this_thread::sleep_for(cfg.p_time);
    while (!q.push(data)) {
      std::this_thread::yield();
    }
  }
}

static void Run_produceN_consumeN(benchmark::State &state) {
  using namespace ex_queue;
  while (state.KeepRunning()) {
    std::vector<int> consumed;
    std::atomic<int> n_task;
    try {
      config conf;
      conf.n_tasks = state.range(0);
      conf.n_threads = 2;
      equeue<std::int64_t> queue(conf);
      n_task = conf.n_tasks;
      for (int nmax = 1; nmax < conf.n_threads; ++nmax) {
        std::vector<std::thread> producers;
        std::vector<std::thread> consumers;
        for (int n = 0; n < nmax; ++n) {
          producers.emplace_back(produce<std::int64_t>, std::ref(queue), conf,
                                 199 + n, std::ref(n_task));
        }
        for (int n = 0; n < nmax; ++n) {
          consumers.emplace_back(consume<std::int64_t>, std::ref(queue), conf,
                                 std::ref(consumed), std::ref(n_task));
        }
        for (auto &p : producers)
          p.join();
        for (auto &c : consumers)
          c.join();
      }
    } catch (std::runtime_error &e) {
      std::cerr << e.what() << std::endl;
    } catch (...) {
      std::cerr << "unknown error" << std::endl;
    }
  }
}

BENCHMARK(Run_produceN_consumeN)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Complexity();
// ################################################################################
//  boost lock_free queue
// ################################################################################

boost::atomic<bool> done(false);

void producer(boost::lockfree::queue<std::int64_t> &queue,
              boost::atomic_int &producer_count, const int iterations) {
  for (int i = 0; i != iterations; ++i) {
    int value = ++producer_count;
    while (!queue.push(value))
      ;
  }
}

void consumer(boost::lockfree::queue<std::int64_t> &queue,
              boost::atomic_int &consumer_count, const int iterations) {
  int value;
  while (!done) {
    while (queue.pop(value))
      ++consumer_count;
  }

  while (queue.pop(value))
    ++consumer_count;
}

static void Run_Boost_produceN_consumeN(benchmark::State &state) {
  while (state.KeepRunning()) {
    boost::lockfree::queue<std::int64_t> queue(1024);
    boost::thread_group producer_threads, consumer_threads;

    boost::atomic_int producer_count(0);
    boost::atomic_int consumer_count(0);

    const int producer_thread_count = 2;
    const int consumer_thread_count = 2;
    const int iterations = state.range(0);
    for (int i = 0; i != producer_thread_count; ++i)
      producer_threads.create_thread(boost::bind(producer, boost::ref(queue),
                                                 boost::ref(producer_count),
                                                 boost::ref(iterations)));

    for (int i = 0; i != consumer_thread_count; ++i)
      consumer_threads.create_thread(boost::bind(consumer, boost::ref(queue),
                                                 boost::ref(consumer_count),
                                                 boost::ref(iterations)));

    producer_threads.join_all();
    done = true;

    consumer_threads.join_all();
  }
}
BENCHMARK(Run_Boost_produceN_consumeN)
    ->Arg(100)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Complexity();

BENCHMARK_MAIN();
