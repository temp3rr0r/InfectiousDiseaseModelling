#include <iostream>
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

void Individual::epoch() {
	if (infected_) {
		if (epochs_infected_ >= parameters_.DiseaseDuration)
			heal();
		else
			++epochs_infected_;
	}
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