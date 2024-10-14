#include <iostream>
#include <unordered_map>

#include "../utils/graph.hpp"
#include "private.hpp"

/* =================Graph===================
 *          inp1
 *            |
 *   const1   n1
 *        \  /
 *         n2
 *        /  \
 *       n3   \
 *        |    n4
 *         \  /  \
 *          n5  others
 *           |
 *        ReadValue
 */
GraphPtr create_test_graph_2(GraphPtr expected_subgraph)
{
    auto graph = createGraph("my_graph");
    NodePtr node;
    std::unordered_map<std::string, NodeType> types;

    types["inp1"] = NodeType::Parameter;
    types["const1"] = NodeType::Const;
    types["ReadValue"] = types["others"] = NodeType::Result;

#define CreateNodeWithName(name)                                                                  \
    auto name = std::make_shared<Node>(types[#name] ? types[#name] : NodeType::Execution, #name); \
    graph->add_node(name)

    CreateNodeWithName(inp1);
    CreateNodeWithName(const1);
    CreateNodeWithName(n1);
    CreateNodeWithName(n2);
    CreateNodeWithName(n3);
    CreateNodeWithName(n4);
    CreateNodeWithName(n5);
    CreateNodeWithName(ReadValue);
    CreateNodeWithName(others);

    // Construct edges to link these nodes.
    CreateEdge(inp1, n1);
    CreateEdge(const1, n2);
    CreateEdge(n1, n2);
    CreateEdge(n2, n3);
    CreateEdge(n2, n4);
    CreateEdge(n3, n5);
    CreateEdge(n4, n5);
    CreateEdge(n4, others);
    CreateEdge(n5, ReadValue);

    graph->show_in_cmd();

    expected_subgraph->clear();
    expected_subgraph->add_node(n2);
    expected_subgraph->add_node(n3);
    expected_subgraph->add_node(n4);
    expected_subgraph->add_node(n5);
    expected_subgraph->add_node(ReadValue);
    return graph;
}