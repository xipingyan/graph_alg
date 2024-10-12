#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <iostream>

class Node;
using NodePtr=std::shared_ptr<Node>;

typedef enum {
    Parmeter = 0,
    Result = 1,
    Const = 2,
    Execution = 3
}NodeType;

class Node : public std::enable_shared_from_this<Node>
{
public:
    Node() = delete;
    Node(NodeType type, std::string name = std::string()) : _type(type), _name(name)
    {
    }
    void add_parent(NodePtr node) {
        _parent.emplace_back(node);
    }
    void add_son(NodePtr node)
    {
        _son.emplace_back(node);
    }

    std::vector<NodePtr> get_parent() {
        return _parent;
    }
    std::vector<NodePtr> get_son()
    {
        return _son;
    }
    std::string get_name() { return _name; }
    std::ostream &operator<<(std::ostream &os)
    {
        os << this->get_name();
        return os;
    }

private:
    std::string _name;
    NodeType _type;
    std::vector<NodePtr> _parent;
    std::vector<NodePtr> _son;
};
inline std::ostream &operator<<(std::ostream &os, const NodePtr &node_ptr)
{
    os << node_ptr->get_name();
    return os;
}

class Graph
{
public:
    Graph(const std::string& name)
    {
        _name = name;
    }
    void add_node(NodePtr node)
    {
        _nodes.emplace_back(node);
    }

    NodePtr get_node(size_t idx) {
        return idx < _nodes.size() ? _nodes[idx] : nullptr;
    }

    size_t get_node_size() {
        return _nodes.size();
    }

    void show_in_cmd()
    {
        std::function<void(NodePtr)> dfs_show = [&](NodePtr node)
        {
            for (auto son : node->get_son())
            {
                std::cout << node << " -> " << son << std::endl;
                dfs_show(son);
            }
        };
        if (get_node_size() == 0u)
        {
            return;
        }
        std::cout << "================================\n";
        dfs_show(_nodes[0]);
        std::cout << "================================\n";
    }

private:
    std::string _name;
    std::vector<NodePtr> _nodes;
};
using GraphPtr=std::shared_ptr<Graph>;
inline GraphPtr createGraph(const std::string& name) {return std::make_shared<Graph>(name);};