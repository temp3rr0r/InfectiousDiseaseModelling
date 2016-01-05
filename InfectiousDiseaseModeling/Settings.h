#pragma once
#include <boost/graph/adjacency_list.hpp>

// Default settings and some custom type definitions

// S means selector
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> LocationUndirectedGraph;

static const bool SAVE_CSV = true;
static const bool SAVE_GRAPHVIZ = false;
static const bool SHOW_EPIDEMIC_RESULTS = true;

static const int DEFAULT_NUMBER_OF_THREADS = 4;

static const std::uint8_t DEFAULT_TOTAL_EPOCHS = 60;
static const int DEFAULT_INDIVIDUAL_COUNT = 400;

static const int INITIAL_INFECTED_COUNT = 15;

static const std::uint8_t DEFAULT_REPEAT_COUNT = 10;

static const int CHUNK_SIZE_DIVIDER = 10;