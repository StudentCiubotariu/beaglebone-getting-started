// Logger.hpp
#pragma once

#include <cstdint>   // std::uint8_t
#include <mutex>
#include <string>

/*****************************************************************************************
 * Logger
 *
 *  - Each Logger has:
 *      • a fixed "name" (std::string, e.g. "MAIN", "Thread 1")
 *      • a minimum log level (uint8-based enum)
 *
 *  - log(level, msg) prints: [LEVEL][NAME] msg
 *    BUT ONLY if level >= minLevel for that logger.
 *
 *  - All Logger instances share one static mutex to protect the output stream.
 *****************************************************************************************/
class Logger
{
public:
    // Log levels with explicit underlying type (uint8_t)
    enum class Level : std::uint8_t
    {
        OFF   = 0,  // no logging
        INFO  = 1,
        WARN  = 2,
        ERROR = 3,
        FATAL = 4
    };

    // Construct a logger with a fixed name and minimum level
    Logger(const std::string& name, Level minLevel = Level::OFF);

    // No copying
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Moving is fine if needed
    Logger(Logger&&) = default;
    Logger& operator=(Logger&&) = default;

    // Log a message with an explicit level
    void log(Level level, const std::string& msg);

    // Convenience helpers that call log(...) with specific levels
    void info (const std::string& msg) { log(Level::INFO,  msg); }
    void warn (const std::string& msg) { log(Level::WARN,  msg); }
    void error(const std::string& msg) { log(Level::ERROR, msg); }
    void fatal(const std::string& msg) { log(Level::FATAL, msg); }

private:
    std::string m_name;                    // logger name
     Level      m_minLevel{ Level::OFF };  // minimum log level

    // One global mutex shared by ALL Logger instances (C++17 inline static)
    inline static std::mutex s_mutex{};

    static const char*  levelToString(Level lvl);
    static std::uint8_t levelValue(Level lvl);
};
