#include <random>
#include "Individual.h"

// Infect the individual
void Individual::infect() {
	infected_ = true;
	hit_ = true;
}

// Recover the individual
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
void Individual::move(std::vector<int>& node_neighbours) {

	node_neighbours.push_back(location_); // Add current location in the new locations vector

	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	std::uniform_int_distribution<> uniform_int_distribution(0, static_cast<int>(node_neighbours.size())); // Since we added the current location, no need to decarase the max int

	location_ = node_neighbours[uniform_int_distribution(mersenne_twister_engine)]; // Assign the random location
}

// Set the location
void Individual::set_location(int location) {
	location_ = location;
}

// Get the location
int Individual::get_location() const {
	return location_;
}

// Check if individual is currently infected
bool Individual::is_infected() const {
	return infected_;
}

// Check if individual was infected in the past
bool Individual::is_hit() const {
	return hit_;
}
