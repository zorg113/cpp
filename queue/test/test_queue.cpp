#define BOOST_TEST_MODULE MyTestSuite
#include "queue.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/test/included/unit_test.hpp>
#include <mutex>
#include <string>
#include <thread>

std::atomic<int> n_task;
std::mutex cons_mut;

/// @brief консьюмер для тестирования много поточной очереди
/// @tparam T тип хранимых двнных
/// @param q сама очерень
/// @param cfg конфигурация для теста
template <typename T>
void consume(ex_queue::equeue<T> &q, ex_queue::config cfg,
             std::vector<int> &consumed) {
  T data;
  while (true) {
    int n = n_task.load();
    if (n < 0 && q.is_empty()) {
      break;
    }
    bool pass = q.pop(data);
    if (pass) {
      std::lock_guard<std::mutex> Lk{cons_mut};
      consumed.push_back(1);
      std::this_thread::sleep_for(cfg.c_time);
    }
  }
}
/// @brief  продьюсер для помещения данных в очередь
/// @tparam T данные
/// @param q очередь
/// @param cfg  конфигурация
template <typename T>
void produce(ex_queue::equeue<T> &q, ex_queue::config cfg, T data) {
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
    // std::cout << "produce::" << data << std::endl;
  }
}

// @brief  продьюсер c остановкой
/// @tparam T данные
/// @param q очередь
/// @param cfg  конфигурация
template <typename T>
void produce_stop(std::stop_token stop, ex_queue::equeue<T> &q,
                  ex_queue::config cfg, T data) {
  while (!stop.stop_requested()) {
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
    // std::cout << "produce::" << data << std::endl;
  }
}
BOOST_AUTO_TEST_SUITE(TestQueue)

BOOST_AUTO_TEST_CASE(Test_equeue_base, *boost::unit_test::enabled()) {
  using namespace ex_queue;
  std::vector<int> consumed;
  try {
    std::filesystem::path path = "./../test/config.json";
    config conf = load_config(path);
    equeue<std::int64_t> queue(conf);
    for (int nmax = 1; nmax < conf.n_threads; ++nmax) {
      n_task = conf.n_tasks;
      std::vector<std::thread> producers;
      std::vector<std::thread> consumers;
      for (int n = 0; n < nmax; ++n) {
        producers.emplace_back(produce<std::int64_t>, std::ref(queue), conf,
                               199 + n);
      }
      for (int n = 0; n < nmax; ++n) {
        consumers.emplace_back(consume<std::int64_t>, std::ref(queue), conf,
                               std::ref(consumed));
      }
      for (auto &p : producers)
        p.join();
      for (auto &c : consumers)
        c.join();
    }
    BOOST_TEST(consumed.size() == (conf.n_tasks + 1) * 9);
    BOOST_TEST(n_task.load() == -1);
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    BOOST_CHECK(false);
  } catch (...) {
    std::cerr << "unknown error" << std::endl;
    BOOST_CHECK(false);
  }
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(Test_equeue_one_producer, *boost::unit_test::enabled()) {
  using namespace ex_queue;
  std::vector<int> consumed;
  try {
    std::filesystem::path path = "./../test/config.json";
    config conf = load_config(path);
    conf.n_tasks = 3;
    n_task = conf.n_tasks;
    equeue<std::int64_t> queue(conf);
    std::thread t1(produce<std::int64_t>, std::ref(queue), conf, 199);
    std::thread t2(consume<std::int64_t>, std::ref(queue), conf,
                   std::ref(consumed));
    std::thread t3(consume<std::int64_t>, std::ref(queue), conf,
                   std::ref(consumed));
    t1.join();
    t2.join();
    t3.join();

    BOOST_TEST(consumed.size() == conf.n_tasks + 1);
    BOOST_TEST(n_task.load() == -1);
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    BOOST_CHECK(false);
  } catch (...) {
    std::cerr << "unknown error" << std::endl;
    BOOST_CHECK(false);
  }
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(Test_equeue_one_consumer, *boost::unit_test::enabled()) {
  using namespace ex_queue;
  std::vector<int> consumed;
  try {
    std::filesystem::path path = "./../test/config.json";
    config conf = load_config(path);
    conf.n_tasks = 3;
    n_task = conf.n_tasks;
    equeue<std::int64_t> queue(conf);
    std::thread t1(produce<std::int64_t>, std::ref(queue), conf, 199);
    std::thread t2(produce<std::int64_t>, std::ref(queue), conf, 299);
    std::thread t3(consume<std::int64_t>, std::ref(queue), conf,
                   std::ref(consumed));
    t1.join();
    t2.join();
    t3.join();
    BOOST_TEST(consumed.size() == conf.n_tasks + 1);
    BOOST_TEST(n_task.load() == -1);
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    BOOST_CHECK(false);
  } catch (...) {
    std::cerr << "unknown error" << std::endl;
    BOOST_CHECK(false);
  }
  BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(Test_serialize, *boost::unit_test::enabled()) {
  using namespace ex_queue;
  using namespace std::literals::chrono_literals;
  std::vector<int> consumed;
  try {
    std::filesystem::path path = "./../test/config.json";
    config conf = load_config(path);
    conf.n_tasks = 3;
    n_task = conf.n_tasks;
    equeue<std::int64_t> queue(conf);
    do {
      std::jthread t1(produce_stop<std::int64_t>, std::ref(queue), conf, 199);
      std::this_thread::sleep_for(3s);
    } while (0);
    std::ofstream ofs("dump.txt");
    boost::archive::text_oarchive oa(ofs);
    oa << queue;
    ofs.close();
    equeue<std::int64_t> queue2(conf);
    std::ifstream ifs("dump.txt");
    boost::archive::text_iarchive ia(ifs);
    ia >> queue2;
    ifs.close();
    BOOST_TEST(queue.operator==(queue2));

  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    BOOST_CHECK(false);
  } catch (...) {
    std::cerr << "unknown error" << std::endl;
    BOOST_CHECK(false);
  }
  BOOST_TEST(true);
}
BOOST_AUTO_TEST_SUITE_END()