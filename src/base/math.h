#ifndef SRC_BASE_MATH_H_
#define SRC_BASE_MATH_H_
#pragma once

#include "base/types.h"

typedef struct {
	U32 min, max;
} Rng1DU32;

typedef struct {
	U64 min, max;
} Rng1DU64;

#define rng1_mag(r) ((r).max - (r).min)
#define rng1_is_mag_zero(r) (rng1_mag(r) == 0)

#endif // SRC_BASE_MATH_H_
