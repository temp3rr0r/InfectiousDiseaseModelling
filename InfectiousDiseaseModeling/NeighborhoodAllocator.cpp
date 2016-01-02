#include <random>
#include "Default.h"
#include "NeighborhoodAllocator.h"

std::map<size_t, std::vector<size_t>> NeighborhoodAllocator::get_node_neighborhood_lookup_map(const LocationUndirectedGraph& location_graph) {
	
	std::map<size_t, std::vector<size_t>> returning_neighborhood_lookup_map;

	LocationUndirectedGraph::vertex_iterator vertex_iterator_start, vertex_iterator_end; // Location node iterators
	std::tie(vertex_iterator_start, vertex_iterator_end) = vertices(location_graph); // Tie iterators with the current graph

	LocationUndirectedGraph::adjacency_iterator neighbour_iterator_start, neighbour_iterator_end; // Neighbouring node iterators
	for (; vertex_iterator_start != vertex_iterator_end; ++vertex_iterator_start) {

		tie(neighbour_iterator_start, neighbour_iterator_end) = adjacent_vertices(*vertex_iterator_start, location_graph); // Tie adjacent/neighbouring location nodes
		std::vector<size_t> current_neighborhood;
		for (; neighbour_iterator_start != neighbour_iterator_end; ++neighbour_iterator_start)
			current_neighborhood.push_back(*neighbour_iterator_start); // Add the current neighbour

		returning_neighborhood_lookup_map[*vertex_iterator_start] = current_neighborhood;
	}

	return returning_neighborhood_lookup_map;
}

// TODO: return "location size_t" vector from graph vertices (by tie of the graph iterators). Must be a distinct list

std::vector<Individual> NeighborhoodAllocator::get_random_individuals(size_t individual_count, size_t location_count) {

	// Generate a population of healthy individuals
	std::vector<Individual> individuals(individual_count, Individual());

	// Randomly assign locations to individuals in the population	
	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	std::uniform_int_distribution<> uniform_int_distribution(0, static_cast<int>(location_count));

	for (Individual& current_individual : individuals) {
		current_individual.set_location(static_cast<size_t>(uniform_int_distribution(mersenne_twister_engine))); // Assign the random location
	}
	return individuals;
}

LocationUndirectedGraph NeighborhoodAllocator::get_sample_location_undirected_graph() {

	LocationUndirectedGraph location_graph;
	enum { a, b, c, d, e, f, g, h, i, j, k };

	add_edge(a, b, location_graph);
	add_edge(a, c, location_graph);

	add_edge(b, c, location_graph);
	add_edge(b, h, location_graph);
	add_edge(b, e, location_graph);

	add_edge(c, h, location_graph);
	add_edge(c, k, location_graph);

	add_edge(d, e, location_graph);

	add_edge(e, f, location_graph);
	add_edge(e, h, location_graph);

	add_edge(f, g, location_graph);
	add_edge(h, i, location_graph);
	add_edge(i, j, location_graph);
	add_edge(j, k, location_graph);

	return location_graph;
}
