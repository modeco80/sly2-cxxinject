#ifndef SIMPLEHEAP_HPP
#define SIMPLEHEAP_HPP

#include <ml/types.h>
#include <stddef.h>

/// A memory heap class. This class does not protect from multithreaded
/// usage; you'll need to do that in a higher layer yourself.
class SimpleHeap {
public:
	typedef usize SizeType;
	typedef ptrdiff_t SignedSizeType;
	typedef u8* Pointer;
	typedef const u8* ConstPointer;

	struct Segment;

	/// Create a heap at a given memory address. This (ab)uses placement new.
	static SimpleHeap* create(u8* begin, SizeType count);

	explicit SimpleHeap(SizeType count);

	/// Allocate some memory
	Pointer allocate(SizeType count);

	/// free it
	void free(Pointer ptr);

private:

	void combineNodes(Segment* seg);

	Segment* head;

	inline Pointer usableChunkStart() { return reinterpret_cast<u8*>(this + 1); }

	inline Pointer usableChunkEnd() { return static_cast<Pointer>(usableChunkStart() + chunkSize); }

	/// Chunk size, in bytes.
	SizeType chunkSize;
};

#endif
