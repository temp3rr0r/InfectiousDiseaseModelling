#include <omp.h>
#include <iostream>
#include "Individual.h"
#include "NeighborhoodAllocator.h"

using namespace std;
using namespace boost;

void simulate_serial(size_t individual_count, size_t location_count, size_t total_epochs) {

	//Generate a graph of location nodes & connections
	LocationUndirectedGraph individual_graph = NeighborhoodAllocator::get_sample_location_undirected_graph(); // TODO: check location count

	// Generate a population of healthy individuals
	vector<Individual> individuals = NeighborhoodAllocator::get_random_individuals(individual_count, location_count);
	
	// Infect a small, random sample of the population
	individuals[0].infect(); // Infect one individual

	// Statistics vector, index is epoch
	vector<std::tuple<size_t, size_t>> epoch_statistics;
	
	// Generate a look up map with the neighbouring nodes for each graph node
	map<size_t, vector<size_t>> neighborhood_lookup_map = NeighborhoodAllocator::get_node_neighborhood_lookup_map(individual_graph);

	// Repeat for all the epochs
	for (size_t current_epoch = 0; current_epoch < (total_epochs + 1); ++current_epoch) {
		
		//	Randomly move all individuals
		for (Individual& current_individual : individuals)
			current_individual.move(neighborhood_lookup_map[current_individual.get_location()]); // Stay in the same spot or move to a neighbouring node
		
		// foreach each individual		
		for (size_t individual_index = 0; individual_index != individuals.size(); ++individual_index) {			
			
			if (individuals[individual_index].is_infected()) { // if the individual is infected

				// and meets another individual that is susceptible the disease
				for (size_t affecting_individual = 0; affecting_individual != individuals.size(); ++affecting_individual) {
					if (individual_index != affecting_individual) {

						// Check if the susceptible individual gets infected
						if (individuals[individual_index].get_location() == individuals[affecting_individual].get_location()) // in the same location
							individuals[affecting_individual].try_infect();						
					}
				}
			}
		}
		
		size_t hit_count = 0;
		size_t infected_count = 0;
		for (Individual& current_individual : individuals) {
			// Check individuals for the number of epochs they're infected and tag them as healed and recovered if a threshold disease_duration is passed
			current_individual.advance_epoch();

			// Gather statistics about the current advance_epoch : what is the fraction of infected and hit individual
			if (current_individual.is_infected())
				++infected_count;
			if (current_individual.is_hit())
				++hit_count;
		}
		epoch_statistics.push_back(std::make_tuple(hit_count, infected_count));
	}

	cout << "Total epochs: " << epoch_statistics.size() << " Individual count: " << individuals.size() << endl;
	for (size_t epoch_index = 0; epoch_index != (total_epochs + 1); ++epoch_index) {
		cout << " Hit[" << epoch_index << "]: " << get<0>(epoch_statistics[epoch_index]) << " Infected[" << epoch_index << "]: " << get<1>(epoch_statistics[epoch_index]) << endl;
	}
}

int main() {	
	omp_set_num_threads(4);

	#pragma omp parallel
	{
		#pragma omp critical
		cout << "Hello from " << omp_get_thread_num() << " thread." << endl;
	}	

	simulate_serial(400, 10, 60); // 400 individuals, 10 locations, 60 epochs

	NeighborhoodAllocator::get_location_undirected_graph_from_file("antwerp.edges");

	system("pause");	
}

