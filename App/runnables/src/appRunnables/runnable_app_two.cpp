/**
 * @file runnable_app_two.cpp
 * @brief Application runnable name and description.
 *
 * This source file defines the top-level runnable functions that are in 
 * represented by threads. These runnables typically trigger specific application
 * behaviors, such as publishing messages or processing data.
 * 
 */
#include "../runnables_internal.hpp"

#include <chrono>
#include <thread>

#include "logger.hpp"

namespace runnables::internal
{
    void runnable_app_two(Publisher /*pub*/, Receiver rx, flow_control::FlowControl& fc)
    {
        Logger log("APP_SUB_A", Logger::Level::INFO);
        uint32_t cnt = 0;

        while (true) {
            fc.wait_turn(flow_control::Id::A);
            if (auto m = rx.try_get_latest()) {
                log.info("A received cycleCounter : " +
                        std::to_string((*m)->header().cyclecounter()));
            }
            fc.done(flow_control::Id::A);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    }
}
