/**
 * @file runnable_app_four.cpp
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
    void runnable_app_four(Publisher /*pub*/, Receiver rx, flow_control::FlowControl& fc)
    {
        Logger log("APP_SUB_C", Logger::Level::INFO);

        while (true)
        {
            fc.wait_turn(flow_control::Id::C);
            if (auto m = rx.try_get_latest())
            {
             log.info("C received cycleCounter : " +
                     std::to_string((*m)->header().cyclecounter()));
            }
            fc.done(flow_control::Id::C);
        }
    }
}
