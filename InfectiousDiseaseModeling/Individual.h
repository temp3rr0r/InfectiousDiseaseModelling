#pragma once
#include <cstdint>
#include <vector>
#include "IndividualParameters.h"

// Individual represents one person that can be infected, healed, infect others and move to other graph node locations
class Individual {
public:		
	Individual() : infected_(false), hit_(false), recovered_(), epochs_infected_(0), location_(0) { } // Default constructor
	Individual(bool infected, bool hit, bool recovered, uint8_t days_infected, int location) // Full constructor
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
private:
	bool infected_;
	bool hit_; // Indicates if individual was infected as some point
	bool recovered_;
	uint8_t epochs_infected_;
	int location_; // Refers to the graph node that represents the current location of the individual
	IndividualParameters parameters_;
};