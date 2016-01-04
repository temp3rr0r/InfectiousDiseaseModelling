#pragma once

// This struct defines the chance for an individual to get infected as well as,
// the infection period in epochs
struct IndividualParameters {
	float Infectiosity = 0.13;
	std::uint8_t DiseaseDuration = 7;
};