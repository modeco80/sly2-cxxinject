#include <ml/types.h>

class XorShift32Random {
	u32 xorshift32_state;

public:
	u32 getSeed() const {
		return xorshift32_state;
	}

	void setSeed(u32 seed) {
		xorshift32_state = seed;
	}

	u32 next() {
		u32 x = xorshift32_state;
		x ^= x << 13;
		x ^= x << 17;
		// most implementations typo this as 5, but it's supposed to be 15
		// https://github.com/WebDrake/xorshift/commit/a482dccf20b763e89114c2e5caafe44af85ca371
		x ^= x >> 15;
		return xorshift32_state = x;
	}

	u32 uniformNext(u32 low, u32 high) {
		const u32 range = high - low + 1;
		const u32 reject_lim = 0xffffffff % range;
		u32 n = 0;
		do {
			n = next();
		} while(n <= reject_lim);
		return (n % range) + low;
	}

	u32 nextBytes(u8* pData, u32 count) {
		if(pData == nil(u8*))
			return 0;

		const u32 nrWords = count / sizeof(u32);
		// stupid penguin bastard
		if(nrWords == 0)
			return 0;

		u32* pWordData = reinterpret_cast<u32*>(pData);
		for(u32 i = 0; i < nrWords; ++i)
			*pWordData++ = next();

		// return back the amount of bytes we wrote
		// we only write words at a time, no unaligned. Sorry peter griffin
		return nrWords * sizeof(u32);
	}
};

static XorShift32Random randomEngine;

void randomInit() {
	randomEngine.setSeed(0x796C694C);
	// TODO hash bx or ai random seed data into this to get
	// a better "random" seed
}

u32 random32() {
	return randomEngine.next();
}

u32 uniformRandom32(u32 low, u32 high) {
	return randomEngine.uniformNext(low, high);
}

float uniformRandomf(float low, float high) {
	// evil.
	union Evil {
		u32 u;
		float f;
	} evil;
	evil.u = randomEngine.next() & 0x7fffff | 0x3f800000;
	return low + (high - low) * (evil.f - 1.0f);
}

u32 randomBytes(u8* pBytes, u32 count) {
	return randomEngine.nextBytes(pBytes, count);
}
