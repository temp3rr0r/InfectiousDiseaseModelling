#pragma once
#include <cstdint>
#include <ostream>
#include "IndividualParameters.h"
#include <vector>

class Individual {
public:		
	Individual() : infected_(false), hit_(false), recovered_(), epochs_infected_(0), location_(0) { } // Default constructor

	Individual(bool infected, bool hit, bool recovered, uint8_t days_infected, size_t location)
		: infected_(infected), hit_(hit), recovered_(recovered), epochs_infected_(days_infected), location_(location) { }

	void infect();
	void heal();
	void epoch();
	void move(std::vector<size_t>& new_locations);
	void set_location(size_t location);
	std::string get_string() const;

private:
	bool infected_;
	bool hit_; // Indicates if individual was infected as some point
	bool recovered_;
	uint8_t epochs_infected_;
	IndividualParameters parameters_;
	size_t location_; // Refers to the graph node that represents the current location of the individual
};

std::ostream& operator<<(const std::ostream& lhs, const std::string& cs);

inline std::ostream& operator<<(std::ostream& input_stream, const Individual& individual) {
	return input_stream << individual.get_string();
}
