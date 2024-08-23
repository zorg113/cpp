#include "mqueue.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <thread>

std::mutex task_mut;

// ################################################################################
//  custom queue
// ################################################################################
/// @brief консьюмер для тестирования много поточной очереди
/// @tparam T тип хранимых двнных
/// @param q сама очерень
/// @param cfg конфигурация для теста
template <typename T>
void consume(mqueue::queue_thread_safe<T> &q, int &n_task) {
  T data;
  while (true) {
    {
      std::lock_guard<std::mutex> lk{task_mut};
      if (n_task < 0)
        break;
    }
    data = q.front();
  }
}
/// @brief  продьюсер для помещения данных в очередь
/// @tparam T данные
/// @param q очередь
/// @param cfg  конфигурация
template <typename T>
void produce(mqueue::queue_thread_safe<T> &q, T data, int &n_task) {
  while (true) {
    {
      std::lock_guard<std::mutex> lk{task_mut};
      if (n_task < 0)
        break;
      n_task -= 1;
    }
    q.push_back(data);
  }
}

static void Run_produceN_consumeN(benchmark::State &state) {
  using namespace mqueue;
  while (state.KeepRunning()) {
    std::vector<int> consumed;
    int n_task;
    try {

      mqueue::queue_thread_safe<std::int64_t> queue;
      n_task = state.range(0);
      for (int nmax = 1; nmax < 5; ++nmax) {
        std::vector<std::thread> producers;
        std::vector<std::thread> consumers;
        for (int n = 0; n < nmax; ++n) {
          producers.emplace_back(produce<std::int64_t>, std::ref(queue),
                                 199 + n, std::ref(n_task));
        }
        for (int n = 0; n < nmax; ++n) {
          consumers.emplace_back(consume<std::int64_t>, std::ref(queue),
                                 std::ref(n_task));
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

BENCHMARK_MAIN();