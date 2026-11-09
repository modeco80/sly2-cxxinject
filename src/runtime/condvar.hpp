#ifndef UTIL_CONDVAR_HPP
#define UTIL_CONDVAR_HPP

#include "mutex.hpp"

class CondVar {

	i32 waiterCount;
	Mutex waiterlock;

	i32 waitSem;

public:
	CondVar();

	// Wait
	void wait(Mutex& mut);

	// Call on another thread to signal one thread to wake up
	void signal();

	void broadcast();
};

#endif
