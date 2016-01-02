#pragma once
#include <map>
#include <vector>
#include "Default.h"
#include "Individual.h"

class NeighborhoodAllocator {
public:
	static std::map<size_t, std::vector<size_t>> get_node_neighborhood_lookup_map(const LocationUndirectedGraph& location_graph);
	static std::vector<Individual> get_random_individuals(size_t individual_count, size_t location_count);
	static LocationUndirectedGraph get_sample_location_undirected_graph();
};