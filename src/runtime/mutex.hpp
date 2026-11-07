#ifndef RUNTIME_MUTEX_HPP
#define RUNTIME_MUTEX_HPP

#include <ml/types.h>

/// A fairly basic mutex.
class Mutex {
	i32 semaId;
	i32 owningThread;
	i32 lockCount;
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();
};

/// A RAII scope class which locks and unlocks a mutex
/// when it loses scope.
class ScopedLock {
	Mutex& mutex;
public:

	explicit ScopedLock(Mutex& mutex)
		: mutex(mutex) {
		mutex.lock();
	}

	~ScopedLock() {
		mutex.unlock();
	}

};

#endif
