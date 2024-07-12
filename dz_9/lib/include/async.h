#pragma once
#include <iostream>
/**
 * @brief  interface async bulk library
 *
 */
namespace async {
/// @brief  class context information about context
class context;
/**
 * @brief create command handler
 *
 * @param bulk_size  number command
 * @param out_stream  stream log output
 * @return context handler
 */
context connect(std::size_t bulk_size, std::ostream &out_stream = std::cout);

/**
 * @brief get command to handler
 *
 * @param ctx handler command
 * @param data data input
 * @param size_data size_data bulk
 */
void recive(context &ctx, const char *data, std::size_t size_data);

/**
 * @brief close handler
 *
 * @param ctx handler
 */
void disconnect(context &ctx);

} // namespace async