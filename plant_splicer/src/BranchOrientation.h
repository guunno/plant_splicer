#pragma once

#include "Vector2.h"
#include "Genome.h"

struct BranchOrientation
{
	Vector2 pos{ 0, 0 }; // Position of Branch
	float dir = 0; // Direction of Branch
	FloatColour colour{ 0, 0, 0 }; // Colour of Branch
	float width = 0; // Width of Branch

	inline BranchOrientation operator* (uint8_t sf) {
		return BranchOrientation{
			pos * sf,
			dir * sf,
			FloatColour { colour.r * sf, colour.r * sf, colour.r * sf },
			width * sf
		};
	}
};
