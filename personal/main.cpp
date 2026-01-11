// main.cpp
#include "Logger.hpp"

#include <thread>
#include <chrono>
#include <string>

void thread_function(const std::string& threadName, Logger::Level minLevel)
{
    // Logger for this thread, with name + minimum level
    Logger log(threadName, minLevel);

    log.info("starting work");

    for (int i = 0; i < 5; ++i)
    {
        if ((i % 2) == 0)
        {
            log.info("hello world (info)");
        }
        else
        {
            log.error("something went wrong (error)");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    log.warn("finished work with some warnings");
}

int main()
{
    // MAIN logger shows everything from INFO upwards
    Logger mainLog("MAIN", Logger::Level::INFO);
    mainLog.info("Starting threads...");

    // Thread 1: minimum level = INFO (prints INFO, WARN, ERROR, FATAL)
    std::thread t1(thread_function, "Thread 1", Logger::Level::INFO);

    // Thread 2: minimum level = ERROR (only ERROR & FATAL)
    std::thread t2(thread_function, "Thread 2", Logger::Level::ERROR);

    t1.join();
    t2.join();

    mainLog.info("All done.");
    return 0;
}
