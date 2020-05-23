#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>


typedef std::recursive_mutex RecursiveMutex;
typedef std::mutex Mutex;
typedef std::lock_guard<std::mutex> GuardLock;
typedef std::unique_lock<std::mutex> UniqueLock;
typedef std::condition_variable Cond;
typedef std::condition_variable_any CondAny;

class spin_mutex
{
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
	spin_mutex() = default;
	spin_mutex(const spin_mutex&) = delete;
	spin_mutex& operator = (const spin_mutex&) = delete;

	void Lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
	}
	bool TryLock()
	{
		if (flag.test_and_set(std::memory_order_acquire))
			return false;
		return true;
	}
	void UnLock()
	{
		flag.clear(std::memory_order_release);
	}
};

