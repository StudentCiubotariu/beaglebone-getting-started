// main.cpp
#include "logger.hpp"
#include "message_types.hpp"
#include "flexible_queue.hpp"

#include <thread>
#include <chrono>
#include <string>

using MsgPtr = std::shared_ptr<Message>;
using MsgQueue = itc::LatestRingBuffer<MsgPtr>;

static void log_ring(Logger& log, const MsgQueue& q)
{
    auto s = q.debug_snapshot();

    std::string line = "RING cap=" + std::to_string(s.capacity)
        + " write=" + std::to_string(s.write_index)
        + " latest=" + std::to_string(s.latest_index)
        + " has=" + std::string(s.has_value ? "true" : "false")
        + " slots=[";

    for (std::size_t i = 0; i < s.slots.size(); ++i) {
        if (i) line += ", ";

        const auto& p = s.slots[i];
        if (!p) {
            line += "_";
        } else {
            line += std::to_string(p->header.cycleCounter);
        }

        if (i == s.latest_index && s.has_value) line += "*";  // mark latest slot
        if (i == s.write_index) line += "W";                  // mark next overwrite slot
    }

    line += "]";
    log.info(line);
}


void thread1(MsgQueue& q, const std::string& threadName, Logger::Level minLevel)
{
    Logger log(threadName, minLevel);

    uint16_t cycleCounter = 0;
    while(true)
    {

        auto msg = q.try_get_latest();
        if (std::nullopt == msg)
        {
            log.info("Nothing to do will continue");
            continue;
        }
        else
        {
            log.info("cycleCounter finished at : " +
                    std::to_string((*msg)->header.cycleCounter));


        }


        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}

static void thread2(MsgQueue& q, const std::string& threadName, Logger::Level minLevel)
{
    Logger log(threadName, minLevel);

    uint16_t cycleCounter = 0;
    while (true)
    {
        auto msg = std::make_shared<Message>();
        msg->header.version = 1;
        msg->header.eSigStatus = 0;
        msg->header.cycleCounter = cycleCounter;
        msg->header.measurementCounter = 0;
        ++msg->header.cycleCounter;
        log.info(std::string("cycleCounter finished at: ") + std::to_string(msg->header.cycleCounter));
        q.publish(std::move(msg));
        // if (auto opt = q.try_get_latest()) {
        //     log.info("cycleCounter finished at : " +
        //             std::to_string((*opt)->header.cycleCounter));
        //}
        log_ring(log, q);
        cycleCounter++;

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

int main()
{
    // MAIN logger shows everything from INFO upwards
    Logger mainLog("MAIN", Logger::Level::INFO);
    mainLog.info("Starting threads...");

    MsgQueue q(3); // buffer capacity 3

    std::thread t1(thread1, std::ref(q), "Thread 1", Logger::Level::INFO);

    std::thread t2(thread2, std::ref(q), "Thread 2", Logger::Level::INFO);

    t1.join();
    t2.join();

    mainLog.info("All done.");
    return 0;
}
