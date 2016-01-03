#pragma once
#include <boost/graph/adjacency_list.hpp>

// Default settings and some custom type definitions

// S means selector
// TODO: check performance listS vs vecS
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> LocationUndirectedGraph;

static const bool SAVE_CSV = false;
static const bool SAVE_GRAPHVIZ = false;
static const bool SHOW_EPIDEMIC_RESULTS = false;

static const int DEFAULT_NUMBER_OF_THREADS = 4;

static const size_t DEFAULT_TOTAL_EPOCHS = 60;
static const size_t DEFAULT_INDIVIDUAL_COUNT = 400;
