/**
 * @file logger.cpp
 * @brief Logger code for severity-based message output.
 */
#include "logger.hpp"
#include "mutex.hpp"
#include <iostream>   // std::cout, std::endl

/**
 * @brief Stores the start time.
 */
static auto t0 = std::chrono::steady_clock::now();

// Store name and severity threshold; no side effects.
Logger::Logger(const std::string& name, Level minLevel)
    : m_name(name),
      m_minLevel(minLevel)
{
}

// Helper: map enum -> text
const char* Logger::levelToString(Level lvl)
{
    switch (lvl)
    {
        case Level::OFF:   return "OFF";
        case Level::INFO:  return "INFO";
        case Level::WARN:  return "WARN";
        case Level::ERROR: return "ERROR";
        case Level::FATAL: return "FATAL";
        default:           return "UNKNOWN";
    }
}

// Helper: map enum -> numeric severity (uint8_t)
std::uint8_t Logger::levelValue(Level lvl)
{
    return static_cast<std::uint8_t>(lvl);
}

// Log function: prints "[LEVEL][NAME] msg" if level >= minLevel
void Logger::log(Level level, const std::string& msg)
{
    if (levelValue(level) > levelValue(m_minLevel)) return;

    auto now = std::chrono::steady_clock::now();
    auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(now - t0).count();

    std::lock_guard<std::mutex> lock(MutexSingleton::instance());
    std::cout << ms << "ms "
              << "[" << levelToString(level)
              << "][" << m_name << "] "
              << msg << "\n";          // note: no std::endl
}
