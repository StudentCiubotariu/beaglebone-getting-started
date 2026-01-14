#pragma once
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <optional>
#include <utility>
#include <vector>
#include <chrono>

namespace itc {

// Stores the last N published items in circular slots.
// Reader always gets the MOST RECENT published item (not FIFO).
template <class T>
class LatestRingBuffer {
public:

  struct DebugSnapshot {
    std::size_t capacity = 0;
    std::size_t write_index = 0;     // next slot to be overwritten
    std::size_t latest_index = 0;    // where the newest item lives
    bool has_value = false;
    std::vector<T> slots;            // physical order [0..cap-1]
  };

  explicit LatestRingBuffer(std::size_t capacity)
    : cap_(capacity)
    , buf_(capacity) 
    {
      if (cap_ == 0) 
      {
        throw std::invalid_argument("LatestRingBuffer capacity must be > 0");
      }
    }

  LatestRingBuffer(const LatestRingBuffer&) = delete;
  LatestRingBuffer& operator=(const LatestRingBuffer&) = delete;

  // Producer: overwrite next slot
  void publish(T value) {
    {
      std::lock_guard<std::mutex> lk(m_);

      buf_[write_] = std::move(value);
      latest_index_ = write_;
      write_ = (write_ + 1) % cap_;

      has_value_ = true;
    }
  }

  // Non-blocking: returns latest if any exists.
  std::optional<T> try_get_latest() const {
    std::lock_guard<std::mutex> lk(m_);

    if (!has_value_) 
    {
      return std::nullopt;
    }

    return buf_[latest_index_]; // copy (for shared_ptr: cheap)
  }

  // NEW: debug snapshot
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

} // namespace itc
