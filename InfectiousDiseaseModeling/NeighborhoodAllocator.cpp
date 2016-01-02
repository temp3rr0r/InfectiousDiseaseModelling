#include "Default.h"
#include "NeighborhoodAllocator.h"
#include <random>

using namespace std;
using namespace boost;

std::map<size_t, std::vector<size_t>> NeighborhoodAllocator::get_node_neighborhood_lookup_map(const IndividualUndirectedGraph& invidual_graph) {
	
	std::map<size_t, std::vector<size_t>> neighborhood_lookup_map;

	IndividualUndirectedGraph::vertex_iterator vertex_iterator_start, vertex_iterator_end;
	tie(vertex_iterator_start, vertex_iterator_end) = vertices(invidual_graph);

	IndividualUndirectedGraph::adjacency_iterator neighbour_iterator_start, neighbour_iterator_end;
	for (; vertex_iterator_start != vertex_iterator_end; ++vertex_iterator_start) {

		tie(neighbour_iterator_start, neighbour_iterator_end) = adjacent_vertices(*vertex_iterator_start, invidual_graph);
		vector<size_t> current_neighborhood;
		for (; neighbour_iterator_start != neighbour_iterator_end; ++neighbour_iterator_start)
			current_neighborhood.push_back(*neighbour_iterator_start);

		neighborhood_lookup_map[*vertex_iterator_start] = current_neighborhood;
	}

	return neighborhood_lookup_map;
}

std::vector<Individual> NeighborhoodAllocator::get_random_individuals(size_t individual_count, size_t location_count) {

	// Generate a population of healthy individuals
	vector<Individual> individuals(individual_count, Individual());

	// Randomly assign locations to individuals in the population	
	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	uniform_int_distribution<> uniform_int_distribution(0, static_cast<int>(location_count));

	for (Individual& current_individual : individuals) {
		current_individual.set_location(static_cast<size_t>(uniform_int_distribution(mersenne_twister_engine))); // Assign the random location
	}
	return individuals;
}

IndividualUndirectedGraph NeighborhoodAllocator::get_sample_individual_undirected_graph() {

	IndividualUndirectedGraph individual_graph;
	enum { a, b, c, d, e, f, g, h, i, j, k };

	add_edge(a, b, individual_graph);
	add_edge(a, c, individual_graph);

	add_edge(b, c, individual_graph);
	add_edge(b, h, individual_graph);
	add_edge(b, e, individual_graph);

	add_edge(c, h, individual_graph);
	add_edge(c, k, individual_graph);

	add_edge(d, e, individual_graph);

	add_edge(e, f, individual_graph);
	add_edge(e, h, individual_graph);

	add_edge(f, g, individual_graph);
	add_edge(h, i, individual_graph);
	add_edge(i, j, individual_graph);
	add_edge(j, k, individual_graph);

	return individual_graph;
}
