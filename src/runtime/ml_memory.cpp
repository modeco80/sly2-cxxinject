#include <ml/abort.h>
#include <sly2/heap.h>
#include "simpleheap.hpp"
#include "utils/log.hpp"
#include "mutex.hpp"

// TODO: at some point we should take over all allocations
// and replace them with the heap.

/// This define controls how much memory we steal from the game's original pointer bump heap,
/// to use for the ml runtime's dynamic heap.
/// The default is to steal ~32 KB of Sly's heap.
#define SLY2_MALLOC_HEAP_SIZE 32*(1024)

extern "C" int _end;

SimpleHeap* g_slyHeap = nil(SimpleHeap*);

/// Mutex protecting access to the dynamic heap. This should allow code using it
/// to run across multiple threads, with a *bit* of a performance drop, since
/// allocations are serialized.
Mutex g_slyHeapMutex;

// Implement malloc() and free(). These will be aliased by the linekr.

extern "C" void* malloc(u32 count) {
	mlASSERT(g_slyHeap != nil(SimpleHeap*) && "sly dynamic heap not initialized?");
	ScopedLock lk(g_slyHeapMutex);
	return reinterpret_cast<void*>(g_slyHeap->allocate(count));
}

extern "C" void free(void* pointer) {
	if(pointer != vnil) {
		ScopedLock lk(g_slyHeapMutex);
		g_slyHeap->free(reinterpret_cast<u8*>(pointer));
	}
}

extern "C" void slyHeapInit() {
	// Init the Sly memory heap.
	s_pvGlobalMin = &_end;
	s_pvHeapMin = &_end;
	g_pvHeapMax = (void*)0x2000000;

	// Init stack
	s_pvStackMin = (void*)0x2000000;
	s_ipvStackCur = -1;

	CritSectInit(&s_critsectStack);

	// Request memory from the Sly 2 heap. We use this memory
	// for our dynamic heap.
	AllocReq req;
	req.size = sizeof(SimpleHeap) + SLY2_MALLOC_HEAP_SIZE;
	req.align = 16;
	void* pvHeapMemory = PvAllocClear(req);
	if(pvHeapMemory == vnil) {
		mlAbort("slyHeapInit(): PvAllocClear() returned a nil pointer");
	}

	// Create the heap from the Sly 2 heap memory.
	g_slyHeap = SimpleHeap::create(reinterpret_cast<u8*>(pvHeapMemory), SLY2_MALLOC_HEAP_SIZE);

	// Now, for a bit of a hack:
	// The Sly 2 memory allocator is repeatedly reset.
	// A rather obnoxious instance of this happening is when another world is loaded.
	//
	// Thankfully, we have a somewhat elegant, if dumb and hacky, solution:
	// After we initalize our heap, bump s_pvGlobalMin up to the end of it.
	// After this is done, the game resetting the heap will never touch our dynamic heap.
	void* pvDynamicHeapEnd = reinterpret_cast<void*>(reinterpret_cast<u8*>(g_slyHeap) + (sizeof(SimpleHeap) + SLY2_MALLOC_HEAP_SIZE));
	s_pvGlobalMin = pvDynamicHeapEnd;
	s_pvHeapMin = pvDynamicHeapEnd;

	utilLogf(LogInfo, "slyHeapInit(): New Sly heap start is is at 0x%08x.", s_pvGlobalMin);
}



