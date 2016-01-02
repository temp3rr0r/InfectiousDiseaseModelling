#include <omp.h>
#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <boost/graph/adjacency_list.hpp> 
#include <boost/graph/depth_first_search.hpp>
#include "Individual.h"
#include "NeighborhoodAllocator.h"

using namespace std;
using namespace boost;

void simulate(bool generate) {

	const size_t N = 400;
	const size_t max_vertices = 10;
	const size_t simulation_days = 60;
	IndividualUndirectedGraph individual_graph;

	//	Read or generate a graph of locations & their connections
	if (generate)
		individual_graph = NeighborhoodAllocator::get_sample_individual_undirected_graph();

	// Generate a population of healthy individuals
	vector<Individual> individuals = NeighborhoodAllocator::get_random_individuals(max_vertices, N);
	
	// Infect a small, random sample of the population
	individuals[0].infect(); // Infect one individual

	// Statistics vector, index is advance_epoch
	vector<std::tuple<size_t, size_t>> epoch_statistics;
	
	// Generate map with the neighborhoods for each graph node
	map<size_t, vector<size_t>> neighborhood_lookup_map = NeighborhoodAllocator::get_node_neighborhood_lookup_map(individual_graph);

	// Repeat for the number of epochs :
	for (size_t current_epoch = 0; current_epoch < (simulation_days + 1); ++current_epoch) {
		
		//	Randomly move all individuals
		for (Individual current_individual : individuals)
			current_individual.move(neighborhood_lookup_map[current_individual.get_location()]); // Stay in the same spot or move to a node neighborhood
		
		// For each location handle the interactions between all individuals at that location			
		for (size_t individual_index = 0; individual_index != individuals.size(); ++individual_index) {			
			// When an individual that is infected
			if (individuals[individual_index].is_infected()) {

				// Meets another individual that is susceptible the disease
				for (size_t affecting_individual = 0; affecting_individual != individuals.size(); ++affecting_individual) {
					if (individual_index != affecting_individual) {

						// Check if the susceptible gets infected
						if (individuals[individual_index].get_location() == individuals[affecting_individual].get_location())
							individuals[affecting_individual].try_infect();						
					}
				}
			}
		}


		//	All individuals that share the same location meet each other.
		size_t hit_count = 0;
		size_t infected_count = 0;
		for (Individual current_individual : individuals) {
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
}

int main() {	
	omp_set_num_threads(4);

	// bool do_use_parallel = false;
	//#pragma omp parallel if(do_use_parallel)
	#pragma omp parallel
	{
		#pragma omp critical
		cout << "Hello from " << omp_get_thread_num() << " thread." << endl;
	}
	
	//boost_test_lambda();
	//traversing_undirected_graph_bgl();
	//dfs_with_bgl();

	simulate(true);

	system("pause");	
}

