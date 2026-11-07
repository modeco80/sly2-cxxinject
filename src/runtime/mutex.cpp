#include "mutex.hpp"
#include <ml/string.h>
#include "eekernel.h"

Mutex::Mutex() {
	SemaParam p;
	memset(&p, 0, sizeof(p));
	p.initCount = 1;
	p.maxCount = 1;
	this->semaId = CreateSema(&p);

	// reset lock state
	this->owningThread = -1;
	this->lockCount = 0;
}

Mutex::~Mutex() {
	DeleteSema(this->semaId);
}

void Mutex::lock() {
	i32 thisThread = GetThreadId();
	if(thisThread == this->owningThread) {
		this->lockCount++;
	} else {
		WaitSema(this->semaId);
		this->owningThread = thisThread;
		this->lockCount = 1;
	}
}

void Mutex::unlock() {
	if(this->lockCount-- == 0) {
		this->owningThread = -1;
		SignalSema(this->semaId);
	}
}
