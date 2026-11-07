#ifndef SLY2_MOD_SDK_HEAP_H
#define SLY2_MOD_SDK_HEAP_H

#include <ml/types.h>

//
struct AllocReq {
	u32 size;
	u32 align;
};

/// Allocates a chunk of memory, cleared.
void* PvAllocClear(const AllocReq& request);

// Heap variables
extern void* s_pvGlobalMin;
extern void* s_pvHeapMin;
extern int s_ipvStackCur;
extern void* s_pvStackMin;
extern void* g_pvHeapMax;

// Not actually an int, but all we need is the address.
extern int s_critsectStack;

// TODO:
extern "C" void CritSectInit(int*);

#endif
