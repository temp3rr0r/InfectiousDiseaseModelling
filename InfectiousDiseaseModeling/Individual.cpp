#include <random>
#include "Individual.h"

// Check if an individual gets infected by a predefined chance
void Individual::try_infect() {

	if (!infected_) {
		if (get_random_infect_chance() < parameters_.Infectiosity)
			infect();
	}
}

float Individual::get_random_infect_chance() {

	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	std::uniform_real_distribution<> real_random(0, 1);

	return static_cast<float>(real_random(mersenne_twister_engine));
}

int Individual::get_random_location(size_t neighbours_size) {
	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	std::uniform_int_distribution<> uniform_int_distribution(0, static_cast<int>(neighbours_size)); // Since we added the current location, no need to decarase the max int

	return uniform_int_distribution(mersenne_twister_engine);
}

// Randomly move the individual to another location or stay at the same location
void Individual::move(std::vector<int>& node_neighbours) {

	node_neighbours.push_back(location_); // Add current location in the new locations vector
	location_ = node_neighbours[get_random_location(node_neighbours.size())]; // Assign the random location
}