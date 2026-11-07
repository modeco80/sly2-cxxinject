#include "simpleheap.hpp"
#include <new>

enum SegmentType {
	kSegmentFree,
	kSegmentMalloc,
	kSegmentWorldMalloc,
	kSegmentStackMalloc
};

struct __attribute__((aligned(4))) SimpleHeap::Segment {
	// intrusively linked chain of segments
	Segment* Prev;
	Segment* Next;

	// size of this heap segment
	SizeType size;
	bool allocated;
};

SimpleHeap* SimpleHeap::create(u8* begin, SimpleHeap::SizeType size) {
	return new(begin) SimpleHeap(size);
}

SimpleHeap::SimpleHeap(SizeType size) : chunkSize(size) {
	// Initialize the head segment of the heap.
	head = new(usableChunkStart()) Segment;
	head->size = size;
}

const static SimpleHeap::SignedSizeType kSegmentSplitSizeHeuristic = static_cast<SimpleHeap::SignedSizeType>(2 * sizeof(SimpleHeap::Segment));

SimpleHeap::Pointer SimpleHeap::allocate(SizeType count) {
	SimpleHeap::Segment* current = nil(Segment*);
	SimpleHeap::Segment* best = nil(Segment*);

	SignedSizeType difference = 0;
	SignedSizeType bestDifference = 0x7fffffff;

	// Align size to 16 (naive, could probably do this better with bit twiddling)
	count += sizeof(Segment);
	count += count % 16 ? 16 - (count % 16) : 0;

	// Find the allocation that is closest in bytes to this request
	for(current = head; current != nil(Segment*); current = current->Next) {
		difference = current->size - count;
		if(!current->allocated && difference < bestDifference && difference >= 0) {
			best = current;
			bestDifference = difference;
		}
	}

	// Couldn't find a fitting allocation, give up.
	if(best == nil(Segment*)) {
		return nil(Pointer);
	}

	// If the best difference we could come up with was large, split up this segment into two.
	if(bestDifference > kSegmentSplitSizeHeuristic) {
		SimpleHeap::Segment* splitSegment = new(reinterpret_cast<u8*>(best) + count) Segment;
		current = best->Next;
		best->Next = splitSegment;
		best->Next->Next = current;
		best->Next->Prev = best;
		best->Next->size = best->size - count;
		best->size = count;
	}

	// Mark the chunk we just allocated as being allocated (obviously)
	best->allocated = true;
	return reinterpret_cast<Pointer>(reinterpret_cast<u8*>(best) + sizeof(Segment));
}

void SimpleHeap::free(Pointer ptr) {
	if(ptr == nil(Pointer))
		return;

	SimpleHeap::Segment* seg = reinterpret_cast<Segment*>(reinterpret_cast<u8*>(ptr) - sizeof(Segment));
	seg->allocated = false;

	// Try combining nodes.
	combineNodes(seg);
}

void SimpleHeap::combineNodes(Segment* seg) {
	// Try to coalesce segments to the left of us.
	while(seg->Prev != nil(Segment*) && !seg->Prev->allocated) {
		seg->Prev->Next = seg->Next;
		seg->Prev->size += seg->size;
		seg = seg->Prev;
	}

	// Try to coalesce segments to the right of us.
	while(seg->Next != nil(Segment*) && !seg->Next->allocated) {
		seg->size += seg->Next->size;
		seg->Next = seg->Next->Next;
	}
}
