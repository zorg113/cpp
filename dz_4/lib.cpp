#include "lib.h"

#include "version.h"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <vector>

/**
 * @brief  вывод версии
 *
 * @return int номер версии
 */
int version() { return PROJECT_VERSION_PATCH; }


template <typename T> typename std::enable_if<std::is_integral<T>::value>::type
print_ip(const T& ip) {
  auto byte = reinterpret_cast<const uint8_t*>(&ip);
  auto pos = byte + sizeof(T) - 1;
  for (; pos != byte; --pos) {    
    std::cout<<(uint16_t)(*pos)<<".";
  }
  std::cout<<(uint16_t)(*pos)<<"\n";
}


template <typename T>
decltype(begin(std::declval<T>()), end(std::declval<T>()), void())
print_ip(const T& ip) {
  auto it = ip.begin();
  auto size = ip.size() -1;
  for (size_t i =0 ; i< size; ++i, ++it) {    
      std::cout << *it << ".";
  }
  std::cout << *it << "\n";
}

void print_ip(const std::string& ip){
  std::cout<<ip<<"\n";
}
namespace impl {
template< typename Tuple, size_t... k>
std::string print_ip(Tuple ip, std::index_sequence<k...>){
  auto val = { ::print_ip(std::get<k>(ip))... };
  std::string res;
  auto it  = std::begin(val);
  auto end = it+ val.size() -1;
  for(;it!=end; ++it){
    res+=*it +".";
  }
  res+=*it +"\n";
  return res;
}
};

template <typename ...Args>
std::string print_ip(const std::tuple<Args...> & ip){
  constexpr auto size =std::tuple_size<std::tuple<Args...>>::value;
  return impl::print_ip(ip,std::make_index_sequence<size>{});
}


void run_ip() {
  print_ip(int8_t{-1});                   // 255
  print_ip(int16_t{0});                   // 0.0
  print_ip(int32_t{2130706433});          // 127.0.0.1
  print_ip(int64_t{8875824491850138409}); // 123.45.67.89.101.112.131.41
  print_ip(std::string{"Hello, World !"}); // Hello, World!
  print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
  print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
  //print_ip(std::make_tuple(123, 456, 789, 0));    // 123.456.789.0
}
