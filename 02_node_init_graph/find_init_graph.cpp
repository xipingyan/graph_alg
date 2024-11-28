#include <algorithm>
#include <cassert>

#include "private.hpp"

// Find init subgraph of root.
// return: <1,2>
// 1: subgraph
// 2: subgraph's input nodes(they belong to 2 kinds of path[1: to readvalue, 2: to output])
std::pair<GraphPtr, GraphPtr> find_init_graph(GraphPtr graph, NodePtr root) {
    GraphPtr subgraph = createGraph("subgraph");
    GraphPtr subgraph_input = createGraph("subgraph_input");

    int search_num = 0;
    std::unordered_set<NodePtr> flag;
    bool found_result = true; // Find result or out of max depth.
    std::function<void(NodePtr, NodePtr)> dsf = [&](NodePtr node, NodePtr final_succesor){
        search_num++;
        if (flag.find(node) != flag.end()) {
            DEBUG_LOG("  ** " << node << " is flagged, skipped.");
            return;
        }

        DEBUG_LOG("  == check_succesor: " << node << ", search_num=" << search_num);

        if (node->get_type() == NodeType::Result) {
            found_result = true;
            return;
        }

        for (auto son_edge : node->get_son_edges()) {
            if (son_edge->son_node() == final_succesor) {
                return;
            }
            dsf(son_edge->son_node(), final_succesor);
        }

        if (!found_result)
        {
            flag.insert(node);
        }
    };

    std::function<void(NodePtr)> reverseDSF = [&](NodePtr node) {
        search_num++;
        if (node->get_type() == NodeType::Parameter) {
            subgraph_input->add_node(node);
            return;
        }

        DEBUG_LOG("== Start dsf : " << node << ", search_num = " << search_num);
        found_result = false;
        dsf(node, root);
        flag.insert(node);
        DEBUG_LOG("--> found_result : " << found_result);
        if (found_result) {
            subgraph_input->add_node(node);
            return;
        }

        subgraph->add_node(node);
        for (auto parent_edges : node->get_parent_edges()) {
            reverseDSF(parent_edges->parent_node());
        }
    };

    assert(root->get_parent_edges().size() == 1u);
    reverseDSF(root->get_parent_edges()[0]->parent_node());

    std::cout << "== subgraph = ";
    for (auto cand : subgraph->get_all_nodes())
    {
        std::cout << cand << ", ";
    }
    std::cout << std::endl;

    std::cout << "== flag = ";
    for (auto f : flag) {
        std::cout << f << ",";
    }
    std::cout << std::endl;

    std::cout << "== Final search_num = " << search_num << std::endl;
    return {subgraph, subgraph_input};
}

// Find init subgraph of root. 2
// Use 2 flag set
// 1: for path to readvalue;
// 2: for path to output; 
std::pair<GraphPtr, GraphPtr> find_init_graph_2_set(GraphPtr graph, NodePtr root) {
    GraphPtr subgraph = createGraph("subgraph");
    GraphPtr subgraph_input = createGraph("subgraph_input");

    int search_num = 0;
    std::unordered_set<NodePtr> flag_path_to_rv;
    std::unordered_set<NodePtr> flag_path_to_output;

    bool found_result = true; // Find result or out of max depth.
    std::function<void(NodePtr, NodePtr)> dsf = [&](NodePtr node, NodePtr final_succesor){
        search_num++;
        if (flag_path_to_output.find(node) != flag_path_to_output.end()) {
            DEBUG_LOG("  ** " << node << " is flagged to flag_path_to_output, skipped.");
            found_result = true;
            return;
        }

        if (flag_path_to_rv.find(node) != flag_path_to_rv.end())
        {
            DEBUG_LOG("  ** " << node << " is flagged to flag_path_to_rv, skipped.");
            return;
        }

        DEBUG_LOG("  == check_succesor: " << node << ", search_num=" << search_num);
        if (node->get_type() == NodeType::Result) {
            found_result = true;
            return;
        }

        for (auto son_edge : node->get_son_edges()) {
            if (son_edge->son_node() == final_succesor) {
                return;
            }
            dsf(son_edge->son_node(), final_succesor);
        }

        if (found_result)
        {
            flag_path_to_output.insert(node);
        }
    };

    std::function<void(NodePtr)> reverseDSF = [&](NodePtr node) {
        search_num++;
        if (node->get_type() == NodeType::Parameter) {
            subgraph_input->add_node(node);
            return;
        }

        DEBUG_LOG("== Start dsf : " << node << ", search_num = " << search_num);
        found_result = false;
        dsf(node, root);
        DEBUG_LOG("--> dsf: " << node << " finish, found_result : " << found_result);
        if (found_result) {
            subgraph_input->add_node(node);
            flag_path_to_output.insert(node);
            return;
        }

        flag_path_to_rv.insert(node);
        subgraph->add_node(node);
        for (auto parent_edges : node->get_parent_edges()) {
            reverseDSF(parent_edges->parent_node());
        }
    };

    assert(root->get_parent_edges().size() == 1u);
    reverseDSF(root->get_parent_edges()[0]->parent_node());

    std::cout << "== subgraph = ";
    for (auto cand : subgraph->get_all_nodes())
    {
        std::cout << cand << ", ";
    }
    std::cout << std::endl;

    std::cout << "== subgraph_input = ";
    for (auto cand : subgraph_input->get_all_nodes())
    {
        std::cout << cand << ", ";
    }
    std::cout << std::endl;

    std::cout << "== flag_path_to_output = ";
    for (auto f : flag_path_to_output) {
        std::cout << f << ",";
    }
    std::cout << std::endl;

    std::cout << "== flag_path_to_rv = ";
    for (auto f : flag_path_to_rv)
    {
        std::cout << f << ",";
    }
    std::cout << std::endl;

    std::cout << "== Final search_num = " << search_num << std::endl;
    return {subgraph, subgraph_input};
}
