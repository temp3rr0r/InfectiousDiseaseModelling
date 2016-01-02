#include <iostream>
#include "Individual.h"
#include <random>

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

void Individual::epoch() {
	if (infected_) {
		if (epochs_infected_ >= parameters_.DiseaseDuration)
			heal();
		else
			++epochs_infected_;
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

std::string Individual::get_string() const {
	std::string returning_string = "Individual at ";

	returning_string.append("Location"); // TODO: change

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
