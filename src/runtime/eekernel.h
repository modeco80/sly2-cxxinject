#ifndef RUNTIME_EEKERNEL_H
#define RUNTIME_EEKERNEL_H

// Some EE kernel things.

#include <ml/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SemaParam {
	i32 currentCount;
	i32 maxCount;
	i32 initCount;
	i32 numWaitThreads;
	u32 attr;
	u32 option;
};

int GetThreadId();
int CreateSema(SemaParam* param);
void DeleteSema(int sema);
void WaitSema(int sema);
void SignalSema(int sema);

#ifdef __cplusplus
}
#endif

#endif
