#pragma once
#include <boost/graph/adjacency_list.hpp>

// S means selector
// TODO: check performance listS vs vecS
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> LocationUndirectedGraph;