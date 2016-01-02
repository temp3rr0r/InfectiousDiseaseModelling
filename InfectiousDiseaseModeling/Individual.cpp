#include <iostream>
#include <random>
#include <string>
#include "Individual.h"

void Individual::infect() {
	infected_ = true;
	hit_ = true;
}

void Individual::heal() {
	if(infected_) {
		infected_ = false;
		recovered_ = true;
	}
}

void Individual::advance_epoch() {
	if (infected_) {
		if (epochs_infected_ >= parameters_.DiseaseDuration)
			heal();
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

		if (real_random(mersenne_twister_engine) < parameters_.Infectiosity)
			infect();
	}
}

void Individual::move(std::vector<size_t>& new_locations) {

	new_locations.push_back(location_); // Add current location in the new locations vector

	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	std::uniform_int_distribution<> uniform_int_distribution(0, new_locations.size());

	location_ = new_locations[uniform_int_distribution(mersenne_twister_engine)]; // Assign the random location
}

void Individual::set_location(size_t location) {
	location_ = location;
}

size_t Individual::get_location() const {
	return location_;
}

std::string Individual::get_string() const {
	std::string returning_string = "Individual at location: ";

	returning_string.append(std::to_string(location_));

	if (infected_)
		returning_string.append(" infected");
	else
		returning_string.append(" healty");

	if (recovered_)
		returning_string.append(" recovered");

	if (hit_)
		returning_string.append(" hit");

	return returning_string;
}

bool Individual::is_infected() const {
	return infected_;
}
bool Individual::is_hit() const {
	return hit_;
}
