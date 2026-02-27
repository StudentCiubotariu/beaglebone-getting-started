// main.cpp
#include "logger.hpp"
#include "runnables.hpp"

#include <thread>
#include <chrono>
#include <string>
#include <memory>

int main()
{
    // MAIN logger shows everything from INFO upwards
    Logger mainLog("MAIN", Logger::Level::INFO);
    mainLog.info("Starting threads...");

    runnables::startDefault(3);

    mainLog.info("All done.");
    return 0;
}
