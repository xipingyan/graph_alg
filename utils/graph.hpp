#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>
#include <unordered_set>

class Node;
using NodePtr=std::shared_ptr<Node>;
class Edge;
using EdgePtr=std::shared_ptr<Edge>;

typedef enum {
    Unknow = 0,
    Parameter = 1,
    Result = 2,
    Const = 3,
    Execution = 4,
}NodeType;

inline std::ostream &operator<<(std::ostream &os, const NodeType &type)
{
    #define case_item(ITEM) case ITEM: os << #ITEM; break
    switch(type) {
        case_item(Unknow);
        case_item(Parameter);
        case_item(Result);
        case_item(Const);
        case_item(Execution);
    }
    return os;
}

class Node : public std::enable_shared_from_this<Node>
{
public:
    Node() = delete;
    Node(NodeType type, std::string name = std::string()) : _type(type), _name(name)
    {
    }
    void add_parent_edge(EdgePtr edge) {
        _parent_edges.emplace_back(edge);
    }
    void add_son_edge(EdgePtr edge) {
        _son_edges.emplace_back(edge);
    }

    std::vector<EdgePtr> get_parent_edges() {
        return _parent_edges;
    }
    std::vector<EdgePtr> get_son_edges()
    {
        return _son_edges;
    }

    std::string get_name() { return _name; }
    std::ostream &operator<<(std::ostream &os)
    {
        os << this->get_name();
        return os;
    }

    NodeType get_type() { return _type; }
private:
    std::string _name;
    NodeType _type;
    std::vector<EdgePtr> _parent_edges;
    std::vector<EdgePtr> _son_edges;
};
inline std::ostream &operator<<(std::ostream &os, const NodePtr &node_ptr)
{
    os << node_ptr->get_name();
    return os;
}

class Edge : public std::enable_shared_from_this<Edge>
{
public:
    Edge() = delete;
    NodePtr parent_node() {return _parent;};
    NodePtr son_node() {return _son;};
    Edge(NodePtr parent, NodePtr son) : _parent(parent), _son(son)
    {
    }
private:
    NodePtr _parent;
    NodePtr _son;
};

inline std::ostream &operator<<(std::ostream &os, const EdgePtr &edge_ptr)
{
    os << "Edge: " << edge_ptr->parent_node() << " -> " << edge_ptr->son_node();
    return os;
}

inline bool operator==(const NodePtr &lhs, const NodePtr &rhs)
{
    return lhs->get_name() == rhs->get_name();
}

class Graph
{
public:
    Graph(const std::string& name) {
        _name = name;
    }
    void add_node(NodePtr node) {
        _nodes.emplace_back(node);
    }

    NodePtr get_node(size_t idx) {
        return idx < _nodes.size() ? _nodes[idx] : nullptr;
    }

    NodePtr get_node(std::string name) {
        auto it = std::find_if(_nodes.begin(), _nodes.end(), [&name](NodePtr& n){return name == n->get_name();});
        return it == _nodes.end() ? nullptr : *it;
    }

    size_t get_node_size() {
        return _nodes.size();
    }

    void show_in_cmd()
    {
        std::unordered_set<NodePtr> flag;
        std::function<void(NodePtr)> dfs_show = [&](NodePtr node)
        {
            // Check if it has been used.
            if (flag.find(node) == flag.end()) {
                flag.insert(node);  
            } else {
                return;
            }

            for (auto son_edge : node->get_son_edges())
            {
                std::cout << son_edge << std::endl;
                dfs_show(son_edge->son_node());
            }
        };
        if (get_node_size() == 0u)
        {
            return;
        }
        std::cout << "================================\n";
        auto param_nodes = find_all_param_nodes(flag);
        for (auto pn : param_nodes) {
            dfs_show(pn);
        }
        std::cout << "================================\n";
    }

    void show_only_nodes() {
        std::cout << "================================\n";
        for (auto n : _nodes) {
            std::cout << n << std::endl;
        }
        std::cout << "================================\n";
    }

private:
    std::string _name;
    std::vector<NodePtr> _nodes;

    // 1: Find nodes with type Parameter
    // 2: Init flag
    std::vector<NodePtr> find_all_param_nodes(std::unordered_set<NodePtr>& flag) {
        std::vector<NodePtr> param_nodes;
        for (auto n : _nodes) {
            // std::cout << n << ", type = " << n->get_type() << std::endl;
            if (n->get_type() == NodeType::Parameter) {
                param_nodes.emplace_back(n);
            }
        }
        return param_nodes;
    }
};

using GraphPtr=std::shared_ptr<Graph>;
inline GraphPtr createGraph(const std::string& name) {return std::make_shared<Graph>(name);};

inline void CreateEdge(NodePtr parent, NodePtr son) {
    auto edge = std::make_shared<Edge>(parent, son);
    parent->add_son_edge(edge);
    son->add_parent_edge(edge);
};