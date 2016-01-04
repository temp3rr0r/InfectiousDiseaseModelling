#include <random>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/graph/graphviz.hpp>
#include "Settings.h"
#include "GraphHandler.h"

// Scan the location graph and return a map that binds every location with a vector of neighbouring locations
std::map<int, std::vector<int>> GraphHandler::get_node_neighborhood_lookup_map(const LocationUndirectedGraph& location_graph) {
	
	std::map<int, std::vector<int>> returning_neighborhood_lookup_map;

	LocationUndirectedGraph::vertex_iterator vertex_iterator_start, vertex_iterator_end; // Location node iterators
	std::tie(vertex_iterator_start, vertex_iterator_end) = vertices(location_graph); // Tie iterators with the current graph

	LocationUndirectedGraph::adjacency_iterator neighbour_iterator_start, neighbour_iterator_end; // Neighbouring node iterators
	for (; vertex_iterator_start != vertex_iterator_end; ++vertex_iterator_start) {

		tie(neighbour_iterator_start, neighbour_iterator_end) = adjacent_vertices(*vertex_iterator_start, location_graph); // Tie adjacent/neighbouring location nodes
		std::vector<int> current_neighborhood;
		for (; neighbour_iterator_start != neighbour_iterator_end; ++neighbour_iterator_start)
			current_neighborhood.push_back(*neighbour_iterator_start); // Add the current neighbour

		returning_neighborhood_lookup_map[*vertex_iterator_start] = current_neighborhood;
	}

	return returning_neighborhood_lookup_map;
}

// TODO: return "location int" vector from graph vertices (by tie of the graph iterators). Must be a distinct list

// Generate a vector of individuals and assign a random location within the requested ranges
std::vector<Individual> GraphHandler::get_random_individuals(int individual_count, int location_count) {

	// Generate a population of healthy individuals
	std::vector<Individual> individuals(individual_count, Individual());

	// Randomly assign locations to individuals in the population	
	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	std::uniform_int_distribution<> uniform_int_distribution(0, location_count);

	for (Individual& current_individual : individuals) {
		current_individual.set_location(uniform_int_distribution(mersenne_twister_engine)); // Assign the random location
	}
	return individuals;
}

// Read the openstream map edges file and generate a Undirected graph of locations
LocationUndirectedGraph GraphHandler::get_location_undirected_graph_from_file(std::string filename) {

	using namespace std;
	using namespace boost;

	LocationUndirectedGraph location_graph;
		
	ifstream input_file_stream(filename);
	
	if (!input_file_stream.is_open())
		return 1;

	typedef tokenizer<escaped_list_separator<char>> Tokenizer;

	vector<string> string_vector;
	string current_line;

	map<int, int> map_location_to_index;
	int current_index = 0;

	while (getline(input_file_stream, current_line)) {
		Tokenizer tok(current_line);
		string_vector.assign(tok.begin(), tok.end());

		int first_edge = stoull(string_vector[0]);
		int second_edge = stoull(string_vector[1]);

		if (map_location_to_index.find(first_edge) == map_location_to_index.end()) {
			// not found
			map_location_to_index[first_edge] = current_index;
			current_index++;
		}

		if (map_location_to_index.find(second_edge) == map_location_to_index.end()) {
			// not found
			map_location_to_index[second_edge] = current_index;
			current_index++;
		}

		add_edge(map_location_to_index[first_edge], map_location_to_index[second_edge], location_graph);
	}

	input_file_stream.close();

	return location_graph;
}

// Generate a sample location undirected graph, similar to the one given in the python toy example
LocationUndirectedGraph GraphHandler::get_sample_location_undirected_graph() {

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

// Save an Undirected location graph into a graphiz dot file, to disk
void GraphHandler::save_undirected_graph_to_graphviz_file(std::string filename, const LocationUndirectedGraph& location_graph) {

	std::vector<std::string> NameVec(location_graph.m_vertices.size(), "Location"); // Name of the node
	
	std::ofstream dotfile(filename.c_str());
	boost::write_graphviz(dotfile, location_graph,
		boost::make_label_writer(&NameVec[0])
		);

	dotfile.close();
}

// Save the hit and infected counts for each epoch into a csv file, to disk
void GraphHandler::save_epoch_statistics_to_csv(std::string filename, const std::vector<std::tuple<int, int>> epoch_statistics){

	std::ofstream output_csv;
	output_csv.open(std::string(filename));

	// Write columns
	output_csv << "epoch,hitcount,infectedcount" << std::endl;
	
	// Write a line for each epoch
	for (int epoch_index = 0; epoch_index != epoch_statistics.size(); ++epoch_index)
		output_csv << epoch_index << "," << get<0>(epoch_statistics[epoch_index]) << "," << get<1>(epoch_statistics[epoch_index]) << std::endl;

	output_csv.close();
}

// Show the Hit percentage (fraction of the total population that got infected), epidemic peak percentage and the epoch of the epidemic peak
void GraphHandler::show_epidemic_results(int population_count, const std::vector<std::tuple<int, int>> epoch_statistics) {
	
	// Fraction of the population that got infected
	int hit_count = get<0>(epoch_statistics[epoch_statistics.size() - 1]);

	// Epidemic peak %
	int epidemic_peak_size = 0;
	int epidemic_peak_epoch = 0;

	// Write a line for each epoch
	for (int epoch_index = 0; epoch_index != epoch_statistics.size(); ++epoch_index) {
		if (get<1>(epoch_statistics[epoch_index]) > epidemic_peak_size) {
			epidemic_peak_size = get<1>(epoch_statistics[epoch_index]);
			epidemic_peak_epoch = epoch_index;
		}
	}

	std::cout << std::endl << "-- Epidemic Results --" << std::endl;
	std::cout << "Hit: " << static_cast<double>(hit_count) / static_cast<double>(population_count) << " %"<< std::endl;
	std::cout << "Epidemic Peak:" << static_cast<double>(epidemic_peak_size) / static_cast<double>(population_count) << " %" << std::endl;
	std::cout << "Epidemic Peak Epoch: " << epidemic_peak_epoch << std::endl;
}