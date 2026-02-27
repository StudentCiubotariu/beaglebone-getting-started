/**
 * @file flow_control.hpp
 * @brief Phase-based coordination primitive for ordered execution.
 *
 * Provides FlowControl, a small utility that coordinates a set of participants
 * through repeating phases. Each phase defines which participants are allowed
 * to proceed; when all participants in a phase report completion, the next
 * phase becomes active.
 */
#pragma once
#include <condition_variable>
#include <chrono>
#include <mutex>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdint>

namespace flow_control {

/**
 * @brief Participant identifier used by FlowControl.
 */
enum class Id : std::uint8_t { A, B, C };

/**
 * @brief Hash functor for Id to allow use in unordered containers.
 */
struct IdHash {
    std::size_t operator()(Id v) const noexcept {
        return static_cast<std::size_t>(v);
    }
};

/**
 * @class FlowControl
 * @brief Coordinates multiple participants through a fixed sequence of phases.
 *
 * A FlowControl instance is configured with a list of phases. Each phase is a list
 * of Ids allowed to "take a turn". A participant calls wait_turn() to block until
 * it is permitted in the current phase, then calls done() to signal completion.
 *
 * When all expected participants for the current phase have called done(), the
 * controller advances to the next phase (wrapping around to the first).
 *
 * @note This class uses assertions for contract violations and reports timeouts
 *       to stderr before asserting.
 */
class FlowControl {
public:
    /// A phase is an ordered list of participants expected to run in that phase.
    using Phase = std::vector<Id>;

    /**
     * @brief Construct a phase controller.
     *
     * @param phases Sequence of phases. Each phase must be non-empty.
     * @param timeout_each_wait Maximum time wait_turn() will block before timing out.
     *
     * @pre @p phases is not empty and each phase contains at least one Id.
     */
    FlowControl(std::vector<Phase> phases,
                std::chrono::milliseconds timeout_each_wait)
        : phases_(std::move(phases)), timeout_(timeout_each_wait)
    {
        assert(!phases_.empty());
        rebuild_expected_for_current_phase();
    }

    /**
     * @brief Block until @p who is allowed to execute in the current phase.
     *
     * This call returns when @p who is part of the current phase and has not yet
     * completed it. If the wait exceeds the configured timeout, the function
     * reports an error and asserts.
     *
     * @param who Participant requesting a turn.
     */
    void wait_turn(Id who) {
        
        /**
         * Important note: means exclusive locking
         */ 
        std::unique_lock<std::mutex> lk(m_);
        /**
         * Important note:
         * wait_for(...) blocks the current thread until either:
         *  - the predicate becomes true 
         *  - the timeout expires.
         * is allowed in the current phase and has not already completed it.
         */ 
        bool ok = cv_.wait_for(lk, timeout_, [&] {
            /**
             * Important note:
             * Must check both expected_ and done_ to ensure the caller
             * expected_ is an unordered_set<Id, IdHash> containing Ids that are allowed in the current phase
             * .count(who) returns:
             *   - 1 if who is in the set
             *   - 0 otherwise
             */ 
            return expected_.count(who) != 0 && done_.count(who) == 0;
        });

        if (!ok) {
            std::cerr << "[ERROR] Timeout waiting (phase=" << phase_idx_ << ")\n";
            assert(false && "FlowControl timeout");
        }
    }

    /**
     * @brief Mark @p who as finished for the current phase.
     *
     * When all expected participants in the current phase have reported done(),
     * FlowControl advances to the next phase and wakes waiting participants.
     *
     * @param who Participant completing its work for the phase.
     *
     * @pre @p who is part of the current phase.
     * @pre @p who has not already called done() in the current phase.
     */
    void done(Id who) {
        bool notify = false;
        {
            std::lock_guard<std::mutex> lk(m_);

            if (expected_.count(who) == 0) {
                std::cerr << "[ERROR] done() called out of phase (phase=" << phase_idx_ << ")\n";
                assert(false && "done() called out of phase");
            }

            if (!done_.insert(who).second) {
                std::cerr << "[ERROR] done() called twice in same phase (phase=" << phase_idx_ << ")\n";
                assert(false && "double done()");
            }

            if (done_.size() == expected_.size()) {
                advance_phase_unlocked();
                notify = true;
            }
        }

        // I recommend always notifying, but keep your notify flag if you want
        if (notify) {
            cv_.notify_all();
        }
    }

private:
    /// Rebuild expected/done sets for the current phase index.
    void rebuild_expected_for_current_phase() {
        expected_.clear();
        done_.clear();
        for (Id id : phases_[phase_idx_]) expected_.insert(id);
        assert(!expected_.empty());
    }

    /// Advance to the next phase (caller must hold m_).
    void advance_phase_unlocked() {
        phase_idx_ = (phase_idx_ + 1) % phases_.size();
        rebuild_expected_for_current_phase();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    std::vector<Phase> phases_;
    std::chrono::milliseconds timeout_;
    size_t phase_idx_ = 0;
    bool stop_ = false;

    std::unordered_set<Id, IdHash> expected_;
    std::unordered_set<Id, IdHash> done_;
};

} // namespace flow_control
