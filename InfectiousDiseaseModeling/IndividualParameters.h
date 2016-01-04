#pragma once
#include <cstdint>

// This struct defines the chance for an individual to get infected as well as,
// the infection period in epochs
struct IndividualParameters {
	float Infectiosity = 0.13;
	uint8_t DiseaseDuration = 7;
};