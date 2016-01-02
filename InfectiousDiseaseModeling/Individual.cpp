#include <random>
#include "Individual.h"

void Individual::infect() {
	infected_ = true;
	hit_ = true;
}

void Individual::recover() {
	if(infected_) {
		infected_ = false;
		recovered_ = true;
	}
}

// Advanced the time for the current individual. Also check if the individual gets healed
void Individual::advance_epoch() {
	if (infected_) {
		if (epochs_infected_ >= parameters_.DiseaseDuration)
			recover();
		else
			++epochs_infected_;
	}
}

// Check if an individual gets infected by a predefined chance
void Individual::try_infect() {
	
	if (!infected_) {
		std::random_device random_device;
		std::mt19937 mersenne_twister_engine(random_device());
		std::uniform_real_distribution<> real_random(0, 1);

		if (static_cast<float>(real_random(mersenne_twister_engine)) < parameters_.Infectiosity)
			infect();
	}
}

// Randomly move the individual to another location or stay at the same location
void Individual::move(std::vector<size_t>& node_neighbours) {

	node_neighbours.push_back(location_); // Add current location in the new locations vector

	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	std::uniform_int_distribution<> uniform_int_distribution(0, static_cast<int>(node_neighbours.size()));

	location_ = node_neighbours[uniform_int_distribution(mersenne_twister_engine)]; // Assign the random location
}

void Individual::set_location(size_t location) {
	location_ = location;
}

size_t Individual::get_location() const {
	return location_;
}

bool Individual::is_infected() const {
	return infected_;
}
bool Individual::is_hit() const {
	return hit_;
}
