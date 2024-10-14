#pragma once
#include "../utils/graph.hpp"

#define DEBUG_LOG(LOG)  if (std::getenv("DEBUG_LOG")) std::cout << LOG << std::endl

GraphPtr create_test_graph_1(GraphPtr expected_subgraph);
GraphPtr create_test_graph_2(GraphPtr expected_subgraph);

// Find init subgraph of node.
GraphPtr find_init_graph(GraphPtr graph, NodePtr node);