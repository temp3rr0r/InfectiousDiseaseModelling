#pragma once
#include <vector>
#include "IndividualParameters.h"

// Individual represents one person that can be infected, healed, infect others and move to other graph node locations
class Individual {
public:
	Individual() : infected_(false), hit_(false), recovered_(), epochs_infected_(0), location_(0) { } // Default constructor
	Individual(bool infected, bool hit, bool recovered, std::uint8_t days_infected, int location) // Full constructor
		: infected_(infected), hit_(hit), recovered_(recovered), epochs_infected_(days_infected), location_(location) { }
	void infect();
	void recover();
	void advance_epoch();
	void try_infect();
	void move(std::vector<int>& new_locations);
	void set_location(int location);
	int get_location() const;
	bool is_infected() const;
	bool is_hit() const;
	bool is_recovered() const;
private:
	bool infected_;
	bool hit_; // Indicates if individual was infected as some point
	bool recovered_;
	std::uint8_t epochs_infected_;
	int location_; // Refers to the graph node that represents the current location of the individual
	IndividualParameters parameters_;
	static float get_random_infect_chance();
	static int get_random_location(size_t neighbours_size);
};

// Infect the individual
inline void Individual::infect() {
	infected_ = true;
	hit_ = true;
}

// Recover the individual
inline void Individual::recover() {
	if (infected_) {
		infected_ = false;
		recovered_ = true;
	}
}

// Advanced the time for the current individual. Also check if the individual gets healed
inline void Individual::advance_epoch() {
	if (infected_) {
		if (epochs_infected_ >= parameters_.DiseaseDuration)
			recover();
		else
			++epochs_infected_;
	}
}

// Set the location
inline void Individual::set_location(int location) {
	location_ = location;
}

// Get the location
inline int Individual::get_location() const {
	return location_;
}

// Check if individual is currently infected
inline bool Individual::is_infected() const {
	return infected_;
}

// Check if individual was infected in the past
inline bool Individual::is_hit() const {
	return hit_;
}

// Check if individual is recovered
inline bool Individual::is_recovered() const {
	return recovered_;
}