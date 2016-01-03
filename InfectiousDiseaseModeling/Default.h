#pragma once
#include <boost/graph/adjacency_list.hpp>

// Default settings and some custom type definitions

// S means selector
// TODO: check performance listS vs vecS
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> LocationUndirectedGraph;

static const bool SAVE_CSV = true;
static const bool SAVE_GRAPHVIZ = true;
static const bool SHOW_EPIDEMIC_RESULTS = true;