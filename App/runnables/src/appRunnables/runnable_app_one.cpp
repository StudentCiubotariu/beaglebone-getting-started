/**
 * @file runnable_app_one.cpp
 * @brief Application runnable name and description.
 *
 * This source file defines the top-level runnable functions that are in 
 * represented by threads. These runnables typically trigger specific application
 * behaviors, such as publishing messages or processing data.
 * 
 */
#include "../runnables_internal.hpp"

#include <chrono>
#include <memory>
#include <thread>

#include "logger.hpp"

namespace runnables::internal
{
    void runnable_app_one(Publisher pub, Receiver /*rx*/, flow_control::FlowControl& fc)
    {
        Logger log("APP_PUB  ", Logger::Level::INFO);
        uint16_t cnt = 0;

        while (true)
        {

            auto msg = std::make_shared<message_payload_one::Message>();
            // fill msg
            auto* header = msg->mutable_header();
            header->set_cyclecounter(cnt);
            pub.publish(msg);
            cnt++;
            log.info("published cycleCounter : " + std::to_string(header->cyclecounter()));  // optional
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

        }
    }
}
