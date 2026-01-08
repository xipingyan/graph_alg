#include <thread>
#include <chrono>

#include "../utils/node.hpp"
#include "../utils/profiler.hpp"

std::ostream &operator<<(std::ostream &os, const NodeType &type)
{
#define case_item(ITEM) \
    case ITEM:          \
        os << #ITEM;    \
        break
    switch (type)
    {
        case_item(Unknow);
        case_item(Parameter);
        case_item(Result);
        case_item(Const);
        case_item(Execution);
    }
    return os;
}

Node::Node(NodeType type, std::string name) : _type(type), _name(name)
{
}

void Node::add_parent_edge(EdgePtr edge)
{
    _parent_edges.emplace_back(edge);
}
void Node::add_son_edge(EdgePtr edge)
{
    _son_edges.emplace_back(edge);
}

std::vector<EdgePtr> Node::get_parent_edges()
{
    return _parent_edges;
}
std::vector<EdgePtr> Node::get_son_edges()
{
    return _son_edges;
}

void Node::execute() {
    PROFILE(P, "execute_" + get_name());
    std::this_thread::sleep_for(std::chrono::milliseconds(_tm));
}

std::string Node::get_name() { return _name; }

NodeType Node::get_type() { return _type; }

std::ostream &operator<<(std::ostream &os, const NodePtr &node_ptr)
{
    os << node_ptr->get_name();
    return os;
}

NodePtr Edge::parent_node() { return _parent; };
NodePtr Edge::son_node() { return _son; };
Edge::Edge(NodePtr parent, NodePtr son) : _parent(parent), _son(son)
{
}

std::ostream &operator<<(std::ostream &os, const EdgePtr &edge_ptr)
{
    os << "Edge: " << edge_ptr->parent_node() << " -> " << edge_ptr->son_node();
    return os;
}

bool operator==(const NodePtr &lhs, const NodePtr &rhs)
{
    return lhs->get_name() == rhs->get_name();
}