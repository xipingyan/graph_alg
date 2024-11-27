#pragma once
#include "../utils/graph.hpp"

#define DEBUG_LOG(LOG)  if (std::getenv("DEBUG_LOG")) std::cout << LOG << std::endl

GraphPtr create_test_graph_1(GraphPtr expected_subgraph);
GraphPtr create_test_graph_2(GraphPtr expected_subgraph);
GraphPtr create_test_graph_3(GraphPtr expected_subgraph);

// Find init subgraph of node.
// My initial algorithm
std::pair<GraphPtr, GraphPtr> find_init_graph(GraphPtr graph, NodePtr node);
std::pair<GraphPtr, GraphPtr> find_init_graph_2_set(GraphPtr graph, NodePtr root);
// Maxsim recommend algorithm
GraphPtr find_init_graph_2(GraphPtr graph, NodePtr root);