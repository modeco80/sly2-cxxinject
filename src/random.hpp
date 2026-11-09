#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <ml/types.h>

void randomInit();

u32 random32();

u32 uniformRandom32(u32 low, u32 high);

float uniformRandomf(float low, float high);

u32 randomBytes(u8* pBytes, u32 count);

#endif
