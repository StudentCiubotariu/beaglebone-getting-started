/**
 * @file runnables.hpp
 * @brief Application runnable entry points.
 *
 * This header declares the top-level runnable functions used to start
 * application execution flows. Runnables typically create and wire
 * communication components and launch worker threads.
 */
#pragma once

#include <cstddef>

namespace runnables
{

/**
 * @brief Start the default application runnable set.
 *
 * Creates the default runtime configuration and launches the associated
 * runnables using a shared connection hub.
 *
 * @param depth
 * Size of the internal message history used by the communication hub.
 * This value controls how many recent messages are retained.
 */
void startDefault(std::size_t depth = 3);

} // namespace runnables
