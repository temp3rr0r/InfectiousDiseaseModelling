#pragma once
#include <cstdint>
#include <vector>
#include "IndividualParameters.h"

class Individual {
public:		
	Individual() : infected_(false), hit_(false), recovered_(), epochs_infected_(0), location_(0) { } // Default constructor
	Individual(bool infected, bool hit, bool recovered, uint8_t days_infected, size_t location) // Full constructor
		: infected_(infected), hit_(hit), recovered_(recovered), epochs_infected_(days_infected), location_(location) { }
	void infect();
	void recover();
	void advance_epoch();
	void try_infect();
	void move(std::vector<size_t>& new_locations);
	void set_location(size_t location);
	size_t get_location() const;
	bool is_infected() const;
	bool is_hit() const;
private:
	bool infected_;
	bool hit_; // Indicates if individual was infected as some point
	bool recovered_;
	uint8_t epochs_infected_;
	size_t location_; // Refers to the graph node that represents the current location of the individual
	IndividualParameters parameters_;
};