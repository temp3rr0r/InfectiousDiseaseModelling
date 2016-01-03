#include <omp.h>
#include <iostream>
#include "Individual.h"
#include "GraphHandler.h"
#include "Settings.h"

using namespace std;
using namespace boost;


void simulate_parallel(size_t individual_count, size_t total_epochs, const LocationUndirectedGraph& individual_graph, vector<Individual>& individuals) {

	// Statistics vector, index is epoch
	vector<std::tuple<size_t, size_t>> epoch_statistics;

	// Generate a look up map with the neighbouring nodes for each graph node
	map<size_t, vector<size_t>> neighborhood_lookup_map = GraphHandler::get_node_neighborhood_lookup_map(individual_graph);

	// Repeat for all the epochs
	for (size_t current_epoch = 0; current_epoch < (total_epochs + 1); ++current_epoch) {

		int index = 0;
		static int max_index = static_cast<int>(individuals.size());
		static int chunk = static_cast<int>(max_index / 10);

		//	Randomly move all individuals
		#pragma omp parallel private(index) shared(individuals, neighborhood_lookup_map, chunk)
		{
			#pragma omp for schedule(dynamic, chunk) nowait
			for (index = 0; index < max_index; index++) {

				Individual current_individual = individuals[index]; // Thread local variable
				size_t current_location = current_individual.get_location(); // Thread local variable
				vector<size_t> neighborhood = neighborhood_lookup_map[current_location]; // Thread local variable
				current_individual.move(neighborhood); // Stay in the same spot or move to a neighbouring node

				individuals[index] = current_individual; // Save individual back to the shared memory space
			}
		}
			
		// foreach each individual
		#pragma omp parallel private(index) shared(individuals, chunk)
		{
			#pragma omp for schedule(dynamic,chunk) nowait
			for (index = 0; index < max_index; index++) {
				Individual current_individual = individuals[index]; // Thread local variable

				if (current_individual.is_infected()) { // if the individual is infected

					// and meets another individual that is susceptible to the disease
					int affecting_index;
					for (affecting_index = 0; affecting_index < max_index; affecting_index++) {

						if (index != affecting_index) {
							Individual affecting_individual = individuals[affecting_index]; // Thread local variable
							// Check if the susceptible individual gets infected
							if (current_individual.get_location() == affecting_individual.get_location()) { // in the same location
								affecting_individual.try_infect();
								individuals[affecting_index] = affecting_individual; // Save affecting individual back to the shared memory space
							}
						}
					}
				}
			}
		}

		//	Randomly move all individuals
		size_t hit_count = 0;
		size_t infected_count = 0;
		#pragma omp parallel private(index) shared(individuals, chunk, infected_count, hit_count)
		{
			#pragma omp for schedule(dynamic,chunk) nowait
			for (index = 0; index < max_index; index++) {
				// Check individuals for the number of epochs they're infected and tag them as healed and recovered if a threshold disease_duration is passed			
				Individual current_individual = individuals[index]; // Thread local variable
				current_individual.advance_epoch();
				// Gather statistics about the current advance_epoch : what is the fraction of infected and hit individual
				if (current_individual.is_infected())
					++infected_count;
				if (current_individual.is_hit())
					++hit_count;
				individuals[index] = current_individual; // Save individual back to the shared memory space
			}
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

void simulate_serial(size_t individual_count, size_t total_epochs, const LocationUndirectedGraph& individual_graph, vector<Individual>& individuals) {
	
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

void reset_input(string filename, size_t individual_count, size_t& location_count, LocationUndirectedGraph& individual_graph, vector<Individual>& individuals) {
	individual_graph = GraphHandler::get_location_undirected_graph_from_file(filename);
	location_count = individual_graph.m_vertices.size();
	individuals = GraphHandler::get_random_individuals(individual_count, location_count);
	individuals[0].infect(); // Infect one individual
}

int main() {

	// Get the default simulation values
	int thread_count = DEFAULT_NUMBER_OF_THREADS;
	size_t individual_count = DEFAULT_INDIVIDUAL_COUNT;
	size_t total_epochs = DEFAULT_TOTAL_EPOCHS;
	int repeat_count = DEFAULT_REPEAT_COUNT;
	string input_graph_filename = "minimumantwerp.edges";

	individual_count *= 10;
	//total_epochs *= 5;
	thread_count = 4;
	//repeat_count *= 4;

	// Set the thread count
	omp_set_num_threads(thread_count);
	
	// Print calculation info
	std::cout << "----- Infectious Diease Modelling -----" << std::endl;
	std::cout << "Number of threads: " << thread_count << std::endl;
	std::cout << "Individual Count: " << individual_count << std::endl;
	std::cout << "Total Epochs: " << total_epochs << std::endl;
	std::cout << "Graph from file: " << input_graph_filename << std::endl;
	std::cout << "Repeat count: " << repeat_count << std::endl;

	// Generate a graph of location nodes & connections
	LocationUndirectedGraph individual_graph;
	size_t location_count;
	// Generate a population of healthy individuals
	vector<Individual> individuals;

	// Generate graph of locations or Read graph of locations from file
	//individual_graph = GraphHandler::get_sample_location_undirected_graph();	
	// OR	
	//individual_graph = GraphHandler::get_location_undirected_graph_from_file("minimumantwerp.edges");
	//individual_graph = GraphHandler::get_location_undirected_graph_from_file("antwerp.edges");

	// Reset individuals
	reset_input(input_graph_filename, individual_count, location_count, individual_graph, individuals);
	std::cout << "Location Count: " << location_count << std::endl; // print info once

	double time_start, time_end, total_time;

	// Serial
	cout << endl << "Running serial...";
	total_time = 0.0;
	for (int current_repeat = 0; current_repeat != repeat_count; ++current_repeat) {
		reset_input(input_graph_filename, individual_count, location_count, individual_graph, individuals); // Reset individuals
		time_start = omp_get_wtime();
		simulate_serial(individual_count, total_epochs, individual_graph, individuals);
		time_end = omp_get_wtime() - time_start;
		total_time += time_end;
		cout << ".";
	}
	cout << (total_time / repeat_count) * 1000.0 << " ms" << endl;

	// OpenMP
	cout << endl << "Running with OpenMP...";
	total_time = 0.0;
	for (int current_repeat = 0; current_repeat != repeat_count; ++current_repeat) {
		reset_input(input_graph_filename, individual_count, location_count, individual_graph, individuals); // Reset individuals
		time_start = omp_get_wtime();
		simulate_parallel(individual_count, total_epochs, individual_graph, individuals);
		time_end = omp_get_wtime() - time_start;
		total_time += time_end;
		cout << ".";
	}
	cout << (total_time / repeat_count) * 1000.0 << " ms" << endl;

	system("pause");	
}

