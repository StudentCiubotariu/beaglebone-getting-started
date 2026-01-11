// Mutex.hpp
#pragma once

#include <mutex>
#include <string>

/***************************************************************************************
 * MutexSingleton
 *
 *  - Exposes exactly one std::mutex for the whole program.
 *  - You call MutexSingleton::instance() to get a reference to it.
 *  - Then you lock it with std::lock_guard or lock/unlock manually.
 ***************************************************************************************/
class MutexSingleton
{
public:
    // Access to the single mutex
    static std::mutex& instance()
    {
        // Function-local static → constructed once, thread-safe in C++11+
        static std::mutex m;
        return m;
    }

    // Prevent creating/copying the class itself
    MutexSingleton() = delete;
    MutexSingleton(const MutexSingleton&) = delete;
    MutexSingleton& operator=(const MutexSingleton&) = delete;
};
