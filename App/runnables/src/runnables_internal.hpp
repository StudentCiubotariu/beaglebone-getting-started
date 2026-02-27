/**
 * @file runnables_internal.hpp
 * @brief Internal runnables for the application
 */
#pragma once

#include "connection_hub.hpp"
#include "message.pb.h"
#include "flow_control.hpp"

namespace runnables::internal
{
    using Hub = connection_hub::ConnectionHub<message_payload_one::Message>;
    using Publisher = Hub::Publisher;
    using Receiver  = Hub::Receiver;

    // implemented in src/appRunnables/*.cpp

    void runnable_app_one(Publisher pub, Receiver rx, flow_control::FlowControl& fc);   // publisher
    void runnable_app_two(Publisher pub, Receiver rx, flow_control::FlowControl& fc);   // receiver A
    void runnable_app_three(Publisher pub, Receiver rx, flow_control::FlowControl& fc); // receiver B
    void runnable_app_four(Publisher pub, Receiver rx, flow_control::FlowControl& fc);  // receiver C
}