#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>
#include <unordered_set>

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

    void execute();

    size_t get_node_size();

    void show_in_cmd();

    void show_only_nodes();

private:
    std::string _name;
    std::vector<NodePtr> _nodes;
    // std::unordered_map<std::string, NodePtr> _nodes;

    // 1: Find nodes with type Parameter
    // 2: Init flag
    std::vector<NodePtr> find_all_param_nodes(std::unordered_set<NodePtr> &flag);
};

using GraphPtr = std::shared_ptr<Graph>;
inline GraphPtr createGraph(const std::string &name) { return std::make_shared<Graph>(name); };

bool IsSameGraph(const GraphPtr &lhs, const GraphPtr &rhs);

void CreateEdge(NodePtr parent, NodePtr son);