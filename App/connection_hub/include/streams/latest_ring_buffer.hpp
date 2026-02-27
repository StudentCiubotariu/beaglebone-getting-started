/**
 * @file latest_ring_buffer.hpp
 * @brief Thread-safe ring buffer retaining only the latest published value.
 */
#pragma once
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>
#include <chrono>
#include <stdexcept>

namespace connection_hub::streams {
/**
 * @class LatestRingBuffer
 * @brief Ring buffer that overwrites old values and exposes the most recent one.
 *
 * LatestRingBuffer stores up to a fixed number of elements but only exposes
 * the most recently published value to consumers. Older values may be
 * overwritten as new data is published.
 *
 * This type is thread-safe for concurrent publishers and receivers.
 *
 * @tparam T Type of element stored in the buffer.
 */
template <class T>
class LatestRingBuffer {
public:
  /**
   * @brief Snapshot of the internal buffer state for debugging purposes.
   *
   * This structure provides a consistent view of the buffer state at a
   * single point in time. It is intended for diagnostics and testing only.
   */
  struct DebugSnapshot {
    std::size_t capacity = 0;
    std::size_t write_index = 0;     // next slot to be overwritten
    std::size_t latest_index = 0;    // where the newest item lives
    bool has_value = false;
    std::vector<T> slots;            // physical order [0..cap-1]
  };

  /**
   * @brief Construct a ring buffer with the given capacity.
   *
   * @param capacity Maximum number of elements retained internally.
   *
   * @throws std::invalid_argument If @p capacity is zero.
   */
  explicit LatestRingBuffer(std::size_t capacity)
    : cap_(capacity)
    , buf_(capacity) 
    {
      if (cap_ == 0) 
      {
        throw std::invalid_argument("LatestRingBuffer capacity must be > 0");
      }
    }

  /// Non-copyable.
  LatestRingBuffer(const LatestRingBuffer&) = delete;
  LatestRingBuffer& operator=(const LatestRingBuffer&) = delete;

  /**
   * @brief Publish a new value into the buffer.
   *
   * The value overwrites the next slot in the ring. After publication,
   * the value becomes visible as the latest element to receivers.
   *
   * @param value Value to publish.
   */
  void publish(T value) {
    {
      std::lock_guard<std::mutex> lk(m_);

      buf_[write_] = std::move(value);
      latest_index_ = write_;
      write_ = (write_ + 1) % cap_;

      has_value_ = true;
    }
  }

  /**
   * @brief Retrieve the most recently published value, if any.
   *
   * This function does not block. If no value has been published yet,
   * it returns std::nullopt.
   *
   * @return Latest value or std::nullopt if the buffer is empty.
   */
  std::optional<T> try_get_latest() const {
    std::lock_guard<std::mutex> lk(m_);

    if (!has_value_) 
    {
      return std::nullopt;
    }

    return buf_[latest_index_]; // copy (for shared_ptr: cheap)
  }

  /**
   * @brief Obtain a snapshot of the internal buffer state.
   *
   * Intended for debugging and diagnostics. The returned snapshot
   * represents a consistent view of the buffer at the time of the call.
   *
   * @return Debug snapshot of the buffer.
   */
  DebugSnapshot debug_snapshot() const {
    std::lock_guard<std::mutex> lk(m_);
    DebugSnapshot s;
    s.capacity = cap_;
    s.write_index = write_;
    s.latest_index = latest_index_;
    s.has_value = has_value_;
    s.slots = buf_; // copies the slots; for shared_ptr this is cheap
    return s;
  }

private:
  const std::size_t cap_;
  mutable std::mutex m_;
  std::vector<T> buf_;

  std::size_t write_ = 0;
  std::size_t latest_index_ = 0;
  bool has_value_ = false;
};

} // namespace connection_hub::streams 
