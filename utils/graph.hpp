#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>
#include <unordered_set>

class Node;
using NodePtr = std::shared_ptr<Node>;
class Edge;
using EdgePtr = std::shared_ptr<Edge>;

typedef enum
{
    Unknow = 0,
    Parameter = 1,
    Result = 2,
    Const = 3,
    Execution = 4,
} NodeType;
std::ostream &operator<<(std::ostream &os, const NodeType &type);

class Node : public std::enable_shared_from_this<Node>
{
public:
    Node() = delete;
    Node(NodeType type, std::string name = std::string());
    void add_parent_edge(EdgePtr edge);
    void add_son_edge(EdgePtr edge);

    std::vector<EdgePtr> get_parent_edges();
    std::vector<EdgePtr> get_son_edges();

    void execute();

    std::string get_name();
    std::ostream &operator<<(std::ostream &os)
    {
        os << this->get_name();
        return os;
    }

    NodeType get_type();

    void set_execute_time(const int &tm /*ms*/) { _tm = tm; }

private:
    std::string _name;
    NodeType _type;
    std::vector<EdgePtr> _parent_edges;
    std::vector<EdgePtr> _son_edges;
    int _tm = 0;
};
std::ostream &operator<<(std::ostream &os, const NodePtr &node_ptr);
class Edge : public std::enable_shared_from_this<Edge>
{
public:
    Edge() = delete;
    NodePtr parent_node();
    NodePtr son_node();
    Edge(NodePtr parent, NodePtr son);

private:
    NodePtr _parent;
    NodePtr _son;
};

std::ostream &operator<<(std::ostream &os, const EdgePtr &edge_ptr);
bool operator==(const NodePtr &lhs, const NodePtr &rhs);

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