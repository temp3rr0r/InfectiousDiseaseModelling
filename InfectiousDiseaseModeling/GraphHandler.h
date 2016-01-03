#pragma once
#include <map>
#include <vector>
#include "Settings.h"
#include "Individual.h"

// GraphHandler contains only static methods that: Show the epidemic results, save statics to csv, save location graphs to graphviz dot files,
// generate undirected location graphs, read undirected location graphs from files, allocate random individuals into a graph and
// generate lookup map for graph node neighborhoods
class GraphHandler {
public:
	static std::map<size_t, std::vector<size_t>> get_node_neighborhood_lookup_map(const LocationUndirectedGraph& location_graph);
	static std::vector<Individual> get_random_individuals(size_t individual_count, size_t location_count);
	static LocationUndirectedGraph get_location_undirected_graph_from_file(std::string filename);
	static LocationUndirectedGraph get_sample_location_undirected_graph();
	static void save_undirected_graph_to_graphviz_file(std::string filename, const LocationUndirectedGraph& location_graph);
	static void save_epoch_statistics_to_csv(std::string filename, const std::vector<std::tuple<size_t, size_t>> epoch_statistics);
	static void show_epidemic_results(size_t population_count, const std::vector<std::tuple<size_t, size_t>> epoch_statistics);
};