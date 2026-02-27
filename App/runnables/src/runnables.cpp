/**
 * @file runnables.cpp
 * @brief Runnables for the application
 */
#include "runnables_internal.hpp"
#include "flow_control.hpp"

#include <memory>
#include <thread>
#include <chrono> 

namespace runnables
{
    void startDefault(std::size_t depth)
    {
        using namespace runnables::internal;

        // Hub must outlive all threads.
        auto hub = std::make_shared<Hub>(depth);

        auto pub = hub->make_publisher();
        auto rx  = hub->make_receiver();

        flow_control::FlowControl fc(
        {
            {flow_control::Id::A, flow_control::Id::B},
            {flow_control::Id::C}
        },
        std::chrono::milliseconds{2000}
        );
        
        // flow_control::FlowControl fc(
        // {
        //     {"APP_SUB_A"},
        //     {"APP_SUB_B"},
        //     {"APP_SUB_C"}
        // },
        // std::chrono::milliseconds{2000}
        // );

        // Start threads. Capture hub to keep it alive.
        std::thread t1([hub, pub, rx, &fc]() mutable { runnables::internal::runnable_app_one(pub, rx, fc); });
        std::thread t2([hub, pub, rx, &fc]() mutable { runnables::internal::runnable_app_two(pub, rx, fc); });
        std::thread t3([hub, pub, rx, &fc]() mutable { runnables::internal::runnable_app_three(pub, rx, fc); });
        std::thread t4([hub, pub, rx, &fc]() mutable { runnables::internal::runnable_app_four(pub, rx, fc); });
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
}

