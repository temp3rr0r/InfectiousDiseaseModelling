#pragma once
#include <boost/pending/property.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

// S means selector
// TODO: check performance listS vs vecS
typedef adjacency_list<listS, vecS, undirectedS> IndividualUndirectedGraph;