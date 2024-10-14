#include <iostream>
#include <unordered_map>

#include "../utils/graph.hpp"
#include "private.hpp"

/* =================Graph===================
 *               inp2
 *                 |
 *        inp1   eltwise
 *           \   /   \
 *   const1 mutmal    n6
 *        \  /  \      |
 *        add    n3    n7
 *         |     |     |
 *         |     n4   ...
 *          \   /      |
 *            n5      n20
 *            |
 *        ReadValue
 */
GraphPtr create_test_graph_1(GraphPtr expected_subgraph)
{
    auto graph = createGraph("my_graph");
    NodePtr node;
    std::unordered_map<std::string, NodeType> types;

    types["inp1"] = types["inp2"] = NodeType::Parameter;
    types["const1"] = NodeType::Const;
    types["ReadValue"] = types["n20"] = NodeType::Result;

#define CreateNodeWithName(name)                                                                \
    auto name = std::make_shared<Node>(types[#name] ? types[#name] : NodeType::Execution, #name); \
    graph->add_node(name)

    CreateNodeWithName(inp1);
    CreateNodeWithName(inp2);
    CreateNodeWithName(eltwise);
    CreateNodeWithName(const1);
    CreateNodeWithName(mutmal);
    CreateNodeWithName(add);
    CreateNodeWithName(n3);
    CreateNodeWithName(n4);
    CreateNodeWithName(n5);
    CreateNodeWithName(ReadValue);

#define CreateNode(name)                                                                  \
    node = std::make_shared<Node>(types[name] ? types[name] : NodeType::Execution, name); \
    graph->add_node(node)

    for (size_t i = 6; i <= 20; i++)
    {
        std::string node_name = std::string("n") + std::to_string(i);
        CreateNode(node_name);
    }

    // Construct edges to link these nodes.
    CreateEdge(inp1, mutmal);
    CreateEdge(inp2, eltwise);
    CreateEdge(eltwise, mutmal);
    CreateEdge(const1, add);
    CreateEdge(mutmal, add);
    CreateEdge(mutmal, n3);
    CreateEdge(n3, n4);
    CreateEdge(add, n5);
    CreateEdge(n4, n5);
    CreateEdge(n5, ReadValue);

    CreateEdge(eltwise, graph->get_node("n6"));
    for (size_t i = 6; i < 20; i++)
    {
        CreateEdge(graph->get_node("n" + std::to_string(i)), graph->get_node("n" + std::to_string(i + 1)));
    }

    graph->show_in_cmd();
    return graph;
}