/**
 * @file connection_hub.hpp
 * @brief Connection hub for message passing
 */
#pragma once
#include <memory>
#include <optional>
#include <cstddef>
#include "streams/latest_ring_buffer.hpp"  // connection_hub::streams::LatestRingBuffer

namespace connection_hub {

/**
 * @class ConnectionHub
 * @brief Core processing hub for message passing between publishers and receivers.
 *
 * @tparam MessageT Type of message being transported.
 */
template <typename MessageT>
class ConnectionHub {
public:
    using MsgPtr   = std::shared_ptr<MessageT>;
    using MsgQueue = connection_hub::streams::LatestRingBuffer<MsgPtr>;

    /**
     * @class Publisher
     * @brief Lightweight handle used to publish messages into a ConnectionHub stream.
     *
     * A Publisher provides write-only access to an internal message queue.
     * It does not own the queue; the lifetime of the queue must exceed the
     * lifetime of the Publisher.
     *
     * This class is intentionally small and cheap to copy or move.
     *
     * @note Thread-safety depends on the underlying MsgQueue implementation.
     */
    class Publisher {
    public:
        /**
         * @brief Construct a Publisher bound to a message queue.
         *
         * @param q Pointer to the message queue used for publishing.
         *          Must not be null and must remain valid for the lifetime
         *          of this Publisher.
         */
        explicit Publisher(MsgQueue* q) : q_(q) {}

        /**
         * @brief Publish a message to the associated queue.
         *
         * The message is forwarded to all subscribers according to the
         * semantics of the underlying queue (e.g. latest-only delivery).
         *
         * @param msg Shared pointer to the message being published.
         */
        void publish(MsgPtr msg) {
            q_->publish(std::move(msg));
        }
    };

    /**
     * @class Receiver
     * @brief Read-only handle for receiving messages from a ConnectionHub.
     *
     * Receiver does not own the underlying queue; it must remain valid
     * for the lifetime of the Receiver.
     */
    class Receiver {
    public:
        explicit Receiver(MsgQueue* q) : q_(q) {}
        /// Retrieve the latest available message, if any.
        std::optional<MsgPtr> try_get_latest() const { return q_->try_get_latest(); } // <-- adjust
    private:
        MsgQueue* q_;
    };

    /// Construct a ConnectionHub with the given queue capacity.
    explicit ConnectionHub(std::size_t capacity)
        : q_(capacity) {}

    /// Create a publisher bound to this hub.
    Publisher make_publisher() { return Publisher(&q_); }
    /// Create a receiver bound to this hub.
    Receiver  make_receiver()  { return Receiver(&q_); }

    private:
    /// Non-owning pointer to the message queue
     MsgQueue* q_;
};

} // namespace connection_hub
