/**
 * @file message_types.hpp
 * @brief Message types for protobuf messages
 */
#pragma once

#include <cstdint>

namespace message_types {
/**
 * @brief Fixed-layout signal header for binary message transport.
 *
 * This structure describes the metadata that precedes every signal payload.
 * The layout is intentionally ordered to avoid padding and ensure
 * alignment-safe access on all supported architectures.
 *
 * All fields use little-endian encoding when serialized.
 */
struct SignalHeader
{
    /**
     * @brief Protocol version of the signal format.
     *
     * Used to allow backward- and forward-compatible decoding.
     * A value of 0 indicates an uninitialized or unknown version.
     */
    uint8_t version;

    /**
     * @brief Signal status indicator.
     *
     * Encodes the current state of the signal (e.g. OK, ERROR, INVALID).
     * Corresponds to the @ref SigStatus enumeration.
     */
    uint8_t eSigStatus;

    /**
     * @brief Originating sensor source.
     *
     * Identifies which sensor produced the signal.
     * Corresponds to the @ref SensorSource enumeration.
     */
    uint8_t eSensorSource;

    /**
     * @brief Reserved for future use.
     *
     * This field is reserved to preserve alignment and allow future
     * extensions without breaking the binary layout.
     * Must be set to 0.
     */
    uint8_t reserved0;

    /**
     * @brief Cycle counter.
     *
     * Monotonically increasing counter representing processing or
     * acquisition cycles. Wraps on overflow.
     */
    uint16_t cycleCounter;

    /**
     * @brief Measurement counter.
     *
     * Counts individual measurements within a cycle or time window.
     * Wraps on overflow.
     */
    uint16_t measurementCounter;
};

/**
 * @struct Message
 * @brief Binary message container.
 *
 * This structure represents a complete binary message consisting of
 * a fixed-size @ref SignalHeader followed by an opaque payload.
 *
 * The payload content and interpretation depend on the signal type
 * and are not interpreted at this layer.
 */
struct Message
{
    /**
     * @brief Signal metadata header.
     *
     * Contains protocol versioning, status information, source identifiers,
     * and counters associated with the payload.
     *
     * @see SignalHeader
     */
    SignalHeader header;

    /**
     * @brief Raw signal payload.
     *
     * Opaque binary payload associated with the signal.
     * The payload size is fixed at compile time.
     */
    uint8_t payload[1000];
};

/**
 * @brief Compile-time verification of binary layout.
 *
 * Ensures that the binary layout of protocol structures remains stable.
 */
static_assert(sizeof(SignalHeader) == 8, "SignalHeader size mismatch");

/**
 * @brief Compile-time verification of message size.
 *
 * Confirms that the complete message size matches the expected
 * wire format.
 */
static_assert(sizeof(Message) == 1008, "Message size mismatch");


} // namespace message_types    