#include <iostream>
#include <unordered_map>

#include "../utils/graph.hpp"
#include "private.hpp"

int main(int argc, char** argv) {

    auto test_graph = [](std::function<GraphPtr(GraphPtr)> create_graph) {
        GraphPtr expected_subgraph = createGraph("expected_graph");
        auto graph = create_graph(expected_subgraph);
        auto subgraph = find_init_graph(graph, graph->get_node("ReadValue"));
        std::cout << "== Found subgraph:" << std::endl;
        subgraph->show_only_nodes();

        std::cout << "== subgraph VS expected = " << IsSameGraph(subgraph, expected_subgraph) << std::endl;
    };

    // test_graph(create_test_graph_1);
    test_graph(create_test_graph_2);

    return EXIT_SUCCESS;
}