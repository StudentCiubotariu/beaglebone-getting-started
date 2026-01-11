// Logger.cpp
#include "Logger.hpp"

#include <iostream>   // std::cout, std::endl

// Constructor: store name and minimum level
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
    // Filter: ignore too-low severity
    if (levelValue(level) > levelValue(m_minLevel))
    {
        return; // drop this message
    }

    std::lock_guard<std::mutex> lock(s_mutex);

    // Only here we use std::cout
    std::cout << "[" << levelToString(level)
              << "][" << m_name << "] "
              << msg << std::endl;
}
