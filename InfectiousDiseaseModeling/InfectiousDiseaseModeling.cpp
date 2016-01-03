#include <omp.h>
#include <iostream>
#include "Individual.h"
#include "GraphHandler.h"
#include "Settings.h"

using namespace std;
using namespace boost;


void simulate_parallel(size_t individual_count, size_t total_epochs, LocationUndirectedGraph individual_graph) {

		size_t location_count = individual_graph.m_vertices.size();

		// Generate a population of healthy individuals
		vector<Individual> individuals = GraphHandler::get_random_individuals(individual_count, location_count);

		// Infect a small, random sample of the population
		individuals[0].infect(); // Infect one individual

								 // Statistics vector, index is epoch
		vector<std::tuple<size_t, size_t>> epoch_statistics;

		// Generate a look up map with the neighbouring nodes for each graph node
		map<size_t, vector<size_t>> neighborhood_lookup_map = GraphHandler::get_node_neighborhood_lookup_map(individual_graph);

		// Repeat for all the epochs
		for (size_t current_epoch = 0; current_epoch < (total_epochs + 1); ++current_epoch) {

			//	Randomly move all individuals
			// for (Individual& current_individual : individuals)
			//		current_individual.move(neighborhood_lookup_map[current_individual.get_location()]); // Stay in the same spot or move to a neighbouring node
			int index = 0;
			int max_index = individuals.size();
			#pragma omp parallel for shared(individuals) private(index, max_index, neighborhood_lookup_map) schedule(static)
			for (index = 0; index < max_index; index++)
				individuals[index].move(neighborhood_lookup_map[individuals[index].get_location()]); // Stay in the same spot or move to a neighbouring node

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

			#pragma omp parallel for shared(individuals) private(index, max_index) schedule(static) reduction(+:infected_count, hit_count)
			for (index = 0; index < max_index; index++) {
				// Check individuals for the number of epochs they're infected and tag them as healed and recovered if a threshold disease_duration is passed
				individuals[index].advance_epoch();

				// Gather statistics about the current advance_epoch : what is the fraction of infected and hit individual
				if (individuals[index].is_infected())
					//#pragma atomic
					++infected_count;
				if (individuals[index].is_hit())
					//#pragma atomic
					++hit_count;
			}

			epoch_statistics.push_back(std::make_tuple(hit_count, infected_count));
		}

		if (SAVE_CSV)
			GraphHandler::save_epoch_statistics_to_csv("output.csv", epoch_statistics);
		if (SAVE_GRAPHVIZ)
			GraphHandler::save_undirected_graph_to_graphviz_file("individualGraph.dot", individual_graph);
		if (SHOW_EPIDEMIC_RESULTS)
			GraphHandler::show_epidemic_results(individual_count, epoch_statistics);
	}


void simulate_serial(size_t individual_count, size_t total_epochs, LocationUndirectedGraph individual_graph) {

	size_t location_count = individual_graph.m_vertices.size();

	// Generate a population of healthy individuals
	vector<Individual> individuals = GraphHandler::get_random_individuals(individual_count, location_count);
	
	// Infect a small, random sample of the population
	individuals[0].infect(); // Infect one individual

	// Statistics vector, index is epoch
	vector<std::tuple<size_t, size_t>> epoch_statistics;
	
	// Generate a look up map with the neighbouring nodes for each graph node
	map<size_t, vector<size_t>> neighborhood_lookup_map = GraphHandler::get_node_neighborhood_lookup_map(individual_graph);

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
	
	if (SAVE_CSV)
		GraphHandler::save_epoch_statistics_to_csv("output.csv", epoch_statistics);
	if (SAVE_GRAPHVIZ)
		GraphHandler::save_undirected_graph_to_graphviz_file("individualGraph.dot", individual_graph);
	if (SHOW_EPIDEMIC_RESULTS)
		GraphHandler::show_epidemic_results(individual_count, epoch_statistics);
}

int main() {

	// Get the default simulation values
	int thread_count = DEFAULT_NUMBER_OF_THREADS;
	size_t individual_count = DEFAULT_INDIVIDUAL_COUNT;
	size_t total_epochs = DEFAULT_TOTAL_EPOCHS;

	// Set the thread count
	omp_set_num_threads(thread_count);
	
	// Print calculation info
	std::cout << "----- Infectious Diease Modelling -----" << std::endl;
	std::cout << "Number of threads: " << thread_count << std::endl;
	std::cout << "Individual Count: " << individual_count << std::endl;
	std::cout << "Total Epochs: " << total_epochs << std::endl;

	LocationUndirectedGraph individual_graph;

	//Generate a graph of location nodes & connections
	//individual_graph = GraphHandler::get_sample_location_undirected_graph();	
	// OR
	// Read graph of locations from file
	individual_graph = GraphHandler::get_location_undirected_graph_from_file("antwerp.edges");
	//individual_graph = GraphHandler::get_location_undirected_graph_from_file("minimumantwerp.edges");

	double time_start, total_time;

	cout << "Running with OpenMP... ";
	time_start = omp_get_wtime();
	simulate_parallel(individual_count, total_epochs, individual_graph);
	total_time = omp_get_wtime() - time_start;
	cout << total_time * 1000.0 << " ms" << endl;

	cout << "Running serial... ";
	time_start = omp_get_wtime();
	simulate_serial(individual_count, total_epochs, individual_graph);
	total_time = omp_get_wtime() - time_start;
	cout << total_time * 1000.0 << " ms" << endl;
	
	//simulate_serial(individual_count * 10, total_epochs, individual_graph); //400 individuals, 60 epochs

	system("pause");	
}

