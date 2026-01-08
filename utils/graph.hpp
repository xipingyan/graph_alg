#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include <oneapi/tbb/flow_graph.h>

#include "../utils/node.hpp"

class Graph
{
public:
    Graph(const std::string &name);
    void clear();
    void add_node(NodePtr node);

    std::vector<NodePtr> get_all_nodes();
    NodePtr get_node(size_t idx);

    NodePtr get_node(std::string name);

    void build();

    void execute();
    void execute_async();

    size_t get_node_size();

    void show_in_cmd();

    void show_only_nodes();

private:
    std::string _name;
    std::vector<NodePtr> _nodes;
    oneapi::tbb::flow::graph _flow_graph; // internal flow graph for async execution
    using FlowNode = oneapi::tbb::flow::continue_node<oneapi::tbb::flow::continue_msg>;
    std::vector<std::unique_ptr<FlowNode>> _flow_nodes; // cached flow nodes
    std::unordered_map<NodePtr, FlowNode*> _node_flow_map; // map from logical node to flow node
    std::unique_ptr<oneapi::tbb::flow::broadcast_node<oneapi::tbb::flow::continue_msg>> _starter;

    // std::unordered_map<std::string, NodePtr> _nodes;

    // 1: Find nodes with type Parameter
    // 2: Init flag
    std::vector<NodePtr> find_all_param_nodes(std::unordered_set<NodePtr> &flag);
};

using GraphPtr = std::shared_ptr<Graph>;
inline GraphPtr createGraph(const std::string &name) { return std::make_shared<Graph>(name); };

bool IsSameGraph(const GraphPtr &lhs, const GraphPtr &rhs);

void CreateEdge(NodePtr parent, NodePtr son);