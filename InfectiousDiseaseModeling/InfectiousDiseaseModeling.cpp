#include <omp.h>
#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <iterator>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp> 
#include <boost/graph/depth_first_search.hpp>
#include "Individual.h"
#include <random>

using namespace std;
using namespace boost;

typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef adjacency_list<listS, vecS, undirectedS, no_property, EdgeWeightProperty> UndirectedGraphWithWeight;
typedef adjacency_list<listS, vecS, undirectedS> UndirectedGraph;
typedef adjacency_list<listS, vecS, undirectedS> IndividualUndirectedGraph;

template <class OutEdgeListS = vecS, class VertexProperty = no_property, class EdgeProperty = no_property,
		class GraphProperty = no_property, class EdgeListS = listS> class adjacency_list { };

class custom_dfs_visitor : public boost::default_dfs_visitor {
public:
	template <typename Vertex, typename Graph> void discover_vertex(Vertex u, const Graph & g) const {
		cout << "At " << u << endl;
	}
	template <typename Edge, typename Graph> void examine_edge(Edge e, const Graph& g) const {
		cout << "Examining edges " << e << endl;
	}
};

void dfs_with_bgl() {
	UndirectedGraphWithWeight graph_with_weight;
	add_edge(0, 1, 8, graph_with_weight);
	add_edge(0, 3, 18, graph_with_weight);
	add_edge(1, 2, 20, graph_with_weight);
	add_edge(2, 3, 2, graph_with_weight);
	add_edge(3, 1, 1, graph_with_weight);
	add_edge(1, 3, 7, graph_with_weight);

	custom_dfs_visitor dfs_visitor;
	depth_first_search(graph_with_weight, visitor(dfs_visitor));
}

void boost_test_lambda() {
	typedef std::istream_iterator<int> istream_iterator;

	std::for_each(
		istream_iterator(cin), istream_iterator(), cout << (boost::lambda::_1 * 3) << " ");
}

void traversing_undirected_graph_bgl() {
	UndirectedGraph simple_graph;
	add_edge(0, 1, simple_graph);
	add_edge(0, 3, simple_graph);
	add_edge(1, 2, simple_graph);
	add_edge(2, 3, simple_graph);

	UndirectedGraph::vertex_iterator vertex_iterator_start, vertex_iterator_end;
	UndirectedGraph::adjacency_iterator neighbour_iterator_start, neighbour_iterator_end;
	tie(vertex_iterator_start, vertex_iterator_end) = vertices(simple_graph);

	for (; vertex_iterator_start != vertex_iterator_end; ++vertex_iterator_start) {
		cout << *vertex_iterator_start << " is connected with ";
		tie(neighbour_iterator_start, neighbour_iterator_end) = adjacent_vertices(*vertex_iterator_start, simple_graph);

		for (; neighbour_iterator_start != neighbour_iterator_end; ++neighbour_iterator_start)
			cout << *neighbour_iterator_start << " ";

		cout << "\n";
	}
}


void simulate(bool generate) {

	IndividualUndirectedGraph invidual_graph;
	const size_t N = 400;
	const size_t max_vertices = 10;
	const size_t simulation_days = 60;

	//	Read or generate a graph of locations & their connections
	if (generate) {
		add_edge(0, 1, invidual_graph);
		add_edge(0, 2, invidual_graph);

		add_edge(1, 2, invidual_graph);
		add_edge(1, 7, invidual_graph);
		add_edge(1, 4, invidual_graph);

		add_edge(2, 7, invidual_graph);
		add_edge(2, max_vertices, invidual_graph);

		add_edge(3, 4, invidual_graph);

		add_edge(4, 5, invidual_graph);
		add_edge(4, 7, invidual_graph);

		add_edge(5, 6, invidual_graph);
		add_edge(7, 8, invidual_graph);
		add_edge(8, 9, invidual_graph);
		add_edge(9, max_vertices, invidual_graph);
	}

	// Check/Compare with the python graph sample (i.e a -> 0, b -> 1 etc)
	UndirectedGraph::vertex_iterator vertex_iterator_start, vertex_iterator_end;
	UndirectedGraph::adjacency_iterator neighbour_iterator_start, neighbour_iterator_end;
	tie(vertex_iterator_start, vertex_iterator_end) = vertices(invidual_graph);

	for (; vertex_iterator_start != vertex_iterator_end; ++vertex_iterator_start) {
		cout << *vertex_iterator_start << " is connected with ";
		tie(neighbour_iterator_start, neighbour_iterator_end) = adjacent_vertices(*vertex_iterator_start, invidual_graph);

		for (; neighbour_iterator_start != neighbour_iterator_end; ++neighbour_iterator_start)
			cout << *neighbour_iterator_start << " ";

		cout << "\n";
	}

	// Generate a population of healthy individuals
	vector<Individual> individuals(N, Individual());
	
	// Infect a small, random sample of the population
	individuals[0].infect(); // Infect one individual

	// Randomly assign locations to individuals in the population	
	std::random_device random_device;
	std::mt19937 mersenne_twister_engine(random_device());
	uniform_int_distribution<> uniform_int_distribution(0, max_vertices);

	for (Individual current_individual : individuals) {
		current_individual.set_location(uniform_int_distribution(mersenne_twister_engine)); // Assign the random location
	}

	// Repeat for the number of epochs :
	for (size_t current_epoch = 0; current_epoch < (simulation_days + 1); ++current_epoch) {
		
		//	Randomly move all individuals :
			// E.g : if an individual is located at a graph node 'a'; its next location is randomly
			// chosen from all graph nodes directly connected to 'a' and the node 'a' itself
			// (i.e: the individual could stay at rest)
		
		// For each location handle the interactions between all individuals at that location :
			//	When an individual that's infected meets and individual that is susceptible the dicease
			// is transmitted with a predefined chance. This chance we will call infectiosity and is
			// usually pretty low (you don't infect every individual you meet).
		
		//	All individuals that share the same location meet each other.
			// Check individuals for the number of epochs they're infected and tag them as healed and recovered
			//if a threshold disease_duration is passed
			// Gather statistics about the current epoch : what is the fraction of infected and hit individual
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

