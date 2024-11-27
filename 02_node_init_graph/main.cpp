#include <iostream>
#include <unordered_map>

#include "../utils/graph.hpp"
#include "private.hpp"

int main(int argc, char** argv) {

    auto test_graph = [](std::function<GraphPtr(GraphPtr)> create_graph, std::function<std::pair<GraphPtr, GraphPtr>(GraphPtr, NodePtr)> find_init_graph) {
        GraphPtr expected_subgraph = createGraph("expected_graph");
        auto graph = create_graph(expected_subgraph);
        auto subgraph = find_init_graph(graph, graph->get_node("ReadValue"));

        std::cout << "== Found subgraph:" << std::endl;
        subgraph.first->show_only_nodes();
        subgraph.second->show_only_nodes();

        std::cout << "== subgraph VS expected = " << IsSameGraph(subgraph.first, expected_subgraph) << std::endl;
    };

    // test_graph(create_test_graph_1, find_init_graph);
    // test_graph(create_test_graph_1, find_init_graph_2);

    // std::cout << "1: Start test my search alogrihm==================================" << std::endl;
    // test_graph(create_test_graph_2, find_init_graph);

    // std::cout << "2: Start test Maxsim's search alogrihm==================================" << std::endl;
    // test_graph(create_test_graph_2, find_init_graph_2);

    std::cout << "1: Start test my search alogrihm==================================" << std::endl;
    // test_graph(create_test_graph_3, find_init_graph);
    test_graph(create_test_graph_3, find_init_graph_2_set);
    
    return EXIT_SUCCESS;
}