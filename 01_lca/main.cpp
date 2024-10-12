#include <iostream>
#include <unordered_map>

#include "../utils/graph.hpp"
#include "private.hpp"

int main(int argc, char** argv) {
    auto graph = createGraph("my_graph");

    /* =================Graph===================
     *                 0
     *               / | \
     *              1  2  3
     *             / \    |
     *            4   5   6
     */
    std::vector<NodePtr> vecNodes;
    std::unordered_map<int, NodeType> types;
    types[0] = NodeType::Parmeter;
    types[4] = types[5] = types[6] = NodeType::Result;
    for (size_t i = 0; i < 7; i++)
    {
        NodePtr node = std::make_shared<Node>(types[i] ? types[i] : NodeType::Execution, std::to_string(i));
        graph->add_node(node);
    }

    // Layer 1
    graph->get_node(0)->add_son(graph->get_node(1));
    graph->get_node(0)->add_son(graph->get_node(2));
    graph->get_node(0)->add_son(graph->get_node(3));

    // Layer 2
    graph->get_node(1)->add_son(graph->get_node(4));
    graph->get_node(1)->add_son(graph->get_node(5));
    // Layer 3
    graph->get_node(3)->add_son(graph->get_node(6));

    graph->show_in_cmd();

    // LCA
#define LDC(x, y, z) std::cout << "== " << #x << " and " << #y << ": LCA = " << lca_1(graph, graph->get_node(x), graph->get_node(y)) << ", expected: " << z << std::endl

    LDC(5, 2, 0);
    LDC(5, 6, 0);
    LDC(3, 6, 3);
    LDC(1, 1, 1);

    return EXIT_SUCCESS;
}