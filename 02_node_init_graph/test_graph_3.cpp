#include <iostream>
#include <unordered_map>

#include "../utils/graph.hpp"
#include "private.hpp"

/* =================Graph===================
 *        inp1
 *         |
 *         n1
 *       / |  \
 *      /  n3  \
 *     n2  | \ |
 *      \  |   n4
 *       \ | /   \
 *         n5   Result2
 *         |
 *      ReadValue
 *         |
 *      Result1
 */
GraphPtr create_test_graph_3(GraphPtr expected_subgraph)
{
    auto graph = createGraph("my_graph");
    NodePtr node;
    std::unordered_map<std::string, NodeType> types;

    types["inp1"] = NodeType::Parameter;
    types["Result1"] = types["Result2"] = NodeType::Result;

#define CreateNodeWithName(name)                                                                  \
    auto name = std::make_shared<Node>(types[#name] ? types[#name] : NodeType::Execution, #name); \
    graph->add_node(name)

    CreateNodeWithName(inp1);
    CreateNodeWithName(n1);
    CreateNodeWithName(n2);
    CreateNodeWithName(n3);
    CreateNodeWithName(n4);
    CreateNodeWithName(n5);
    CreateNodeWithName(ReadValue);
    CreateNodeWithName(Result1);
    CreateNodeWithName(Result2);

    // Construct edges to link these nodes.
    CreateEdge(inp1, n1);
    CreateEdge(n1, n2);
    CreateEdge(n1, n3);
    CreateEdge(n1, n4);
    CreateEdge(n2, n5);
    CreateEdge(n3, n4);
    CreateEdge(n3, n5);
    CreateEdge(n4, n5);
    CreateEdge(n4, Result2);
    CreateEdge(n5, ReadValue);
    CreateEdge(ReadValue, Result1);

    graph->show_in_cmd();

    expected_subgraph->clear();
    expected_subgraph->add_node(n2);
    expected_subgraph->add_node(n5);
    expected_subgraph->add_node(ReadValue);
    return graph;
}