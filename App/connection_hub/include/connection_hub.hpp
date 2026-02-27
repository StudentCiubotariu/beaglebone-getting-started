#pragma once
#include <memory>
#include <optional>
#include <cstddef>
#include "streams/latest_ring_buffer.hpp"

namespace connection_hub {

template <typename MessageT>
class ConnectionHub {
public:
    using MsgPtr   = std::shared_ptr<MessageT>;
    using MsgQueue = connection_hub::streams::LatestRingBuffer<MsgPtr>;

    class Publisher {
    public:
        explicit Publisher(MsgQueue* q) : q_(q) {}

        void publish(MsgPtr msg) {
            q_->publish(std::move(msg));
        }

    private:
        MsgQueue* q_;   // <-- ADD THIS
    };

    class Receiver {
    public:
        explicit Receiver(MsgQueue* q) : q_(q) {}

        std::optional<MsgPtr> try_get_latest() const { return q_->try_get_latest(); }

    private:
        MsgQueue* q_;
    };

    explicit ConnectionHub(std::size_t capacity)
        : q_(capacity) {}     // <-- Now valid because q_ is an object

    Publisher make_publisher() { return Publisher(&q_); }
    Receiver  make_receiver()  { return Receiver(&q_); }

private:
    MsgQueue q_;              // <-- CHANGE from MsgQueue* to MsgQueue
};

} // namespace connection_hub