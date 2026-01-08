#include <thread>
#include <chrono>
#include <unordered_map>

#include "../utils/graph.hpp"
#include "../utils/profiler.hpp"

Graph::Graph(const std::string &name)
{
    _name = name;
}
void Graph::clear() { _nodes.clear(); }
void Graph::add_node(NodePtr node)
{
    _nodes.emplace_back(node);
}

std::vector<NodePtr> Graph::get_all_nodes() { return _nodes; }
NodePtr Graph::get_node(size_t idx)
{
    return idx < _nodes.size() ? _nodes[idx] : nullptr;
}

NodePtr Graph::get_node(std::string name)
{
    auto it = std::find_if(_nodes.begin(), _nodes.end(), [&name](NodePtr &n)
                           { return name == n->get_name(); });
    return it == _nodes.end() ? nullptr : *it;
}

size_t Graph::get_node_size()
{
    return _nodes.size();
}

void Graph::execute() {
    for (auto& node : _nodes) {
		node->execute();
    }
}

void Graph::execute_async() {
    using namespace oneapi::tbb::flow;

    _flow_graph.reset();

    using FlowNode = continue_node<continue_msg>;
    std::vector<std::unique_ptr<FlowNode>> flow_nodes;
    flow_nodes.reserve(_nodes.size());
    std::unordered_map<NodePtr, FlowNode*> node_map;

    for (auto& node : _nodes) {
        flow_nodes.emplace_back(std::make_unique<FlowNode>(
            _flow_graph,
            [node](const continue_msg&) {
                node->execute();
            }));
        node_map[node] = flow_nodes.back().get();
    }

    for (auto& parent_node : _nodes) {
        auto it_parent = node_map.find(parent_node);
        if (it_parent == node_map.end()) {
            continue;
        }
        for (auto& edge : parent_node->get_son_edges()) {
            auto child = edge->son_node();
            auto it_child = node_map.find(child);
            if (it_child != node_map.end()) {
                make_edge(*it_parent->second, *it_child->second);
            }
        }
    }

    broadcast_node<continue_msg> starter(_flow_graph);
    for (auto& node : _nodes) {
        if (node->get_parent_edges().empty()) {
            auto it = node_map.find(node);
            if (it != node_map.end()) {
                make_edge(starter, *it->second);
            }
        }
    }

    starter.try_put(continue_msg{});
    _flow_graph.wait_for_all();
}

void Graph::show_in_cmd()
{
    std::unordered_set<NodePtr> flag;
    std::function<void(NodePtr)> dfs_show = [&](NodePtr node)
    {
        // Check if it has been used.
        if (flag.find(node) == flag.end())
        {
            flag.insert(node);
        }
        else
        {
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
    for (auto pn : param_nodes)
    {
        dfs_show(pn);
    }
    std::cout << "================================\n";
}

void Graph::show_only_nodes()
{
    std::cout << "================================\n";
    for (auto n : _nodes)
    {
        std::cout << n << std::endl;
    }
    std::cout << "================================\n";
}

std::vector<NodePtr> Graph::find_all_param_nodes(std::unordered_set<NodePtr> &flag)
{
    std::vector<NodePtr> param_nodes;
    for (auto n : _nodes)
    {
        // std::cout << n << ", type = " << n->get_type() << std::endl;
        if (n->get_type() == NodeType::Parameter)
        {
            param_nodes.emplace_back(n);
        }
    }
    return param_nodes;
}

bool IsSameGraph(const GraphPtr &lhs, const GraphPtr &rhs)
{
    auto all_nodes_l = lhs->get_all_nodes();
    for (auto node : all_nodes_l)
    {
        auto it = std::find(all_nodes_l.begin(), all_nodes_l.end(), node);
        if (it == all_nodes_l.end())
        {
            return false;
        }
    }
    auto all_nodes_r = rhs->get_all_nodes();
    for (auto node : all_nodes_r)
    {
        auto it = std::find(all_nodes_r.begin(), all_nodes_r.end(), node);
        if (it == all_nodes_r.end())
        {
            return false;
        }
    }

    if (all_nodes_r.size() == 0u && all_nodes_l.size() == 0u)
    {
        return true;
    }
    return true;
}

void CreateEdge(NodePtr parent, NodePtr son)
{
    auto edge = std::make_shared<Edge>(parent, son);
    parent->add_son_edge(edge);
    son->add_parent_edge(edge);
};