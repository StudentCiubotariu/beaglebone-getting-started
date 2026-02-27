/**
 * @file logger.hpp
 * @brief Logger interface for severity-based message output.
 */
#pragma once

#include <cstdint>   // std::uint8_t
#include <string>

/**
 * @class Logger
 * @brief Core processing Logger for message logging with levels.
 * 
 * Logger is movable but not copyable.
 */
class Logger
{
public:
    /**
     * @enum Level
     * @brief Log severity levels.
     *
     * Defines the verbosity/severity of log messages.
     * The enum uses an explicit underlying type of uint8_t
     * to keep storage compact and predictable.
     */
    enum class Level : std::uint8_t
    {
        OFF   = 0,  ///< No logging
        INFO  = 1,  ///< Informational messages
        WARN  = 2,  ///< Warnings
        ERROR = 3,  ///< Recoverable errors
        FATAL = 4   ///< Unrecoverable errors
    };

    /**
     * @brief Construct a logger with a fixed name and minimum log level.
     *
     * @param name      Human-readable name identifying the logger instance.
     * @param minLevel  Minimum severity level required for messages to be emitted.
     *                  Messages with a lower level are discarded.
     */
    Logger(const std::string& name, Level minLevel = Level::OFF);


    /// Logger is non-copyable and non-assignable.
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /// Logger moving is fine if needed.
    Logger(Logger&&) = default;
    Logger& operator=(Logger&&) = default;

    /**
     * @brief Log a message with an explicit severity level.
     *
     * If @p level is below the logger's minimum severity threshold,
     * the message is discarded.
     *
     * @param level Severity level of the message.
     * @param msg   Message text to be logged.
     */
    void log(Level level, const std::string& msg);

    /// Log an informational message.
    void info (const std::string& msg) { log(Level::INFO,  msg); }
    /// Log a warning message.
    void warn (const std::string& msg) { log(Level::WARN,  msg); }
    /// Log an error message.
    void error(const std::string& msg) { log(Level::ERROR, msg); }
    /// Log a fatal error message.
    void fatal(const std::string& msg) { log(Level::FATAL, msg); }

private:
    std::string m_name;                    ///< Logger name
    Level       m_minLevel{ Level::OFF };  ///< Minimum severity threshold

    /// Convert a log level to a human-readable string.
    static const char*  levelToString(Level lvl);
    /// Convert a log level to its numeric value.
    static std::uint8_t levelValue(Level lvl);
};
