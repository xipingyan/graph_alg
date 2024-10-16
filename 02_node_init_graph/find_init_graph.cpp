#include <algorithm>
#include <cassert>

#include "private.hpp"

GraphPtr find_init_graph(GraphPtr graph, NodePtr root) {
    GraphPtr subgraph = createGraph("subgraph");

    int search_num = 0;
    std::unordered_set<NodePtr> flag;
    bool found_result = true; // Find result or out of max depth.
    std::function<void(NodePtr, NodePtr)> check_succesor = [&](NodePtr node, NodePtr final_succesor){
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
            check_succesor(son_edge->son_node(), final_succesor);
        }
    };

    std::function<void(NodePtr)> reverseDSF = [&](NodePtr node) {
        search_num++;
        subgraph->add_node(node);

        // if (node->get_type() == NodeType::Parameter) {
        //     return;
        // }

        // search_num = 0;
        DEBUG_LOG("== Start check sucessor : " << node << ", search_num = " << search_num);
        found_result = false;
        check_succesor(node, root);
        flag.insert(node);
        DEBUG_LOG("--> found_result : " << found_result);
        if (found_result) {
            return;
        }

        for (auto parent_edges : node->get_parent_edges()) {
            reverseDSF(parent_edges->parent_node());
        }
    };

    assert(root->get_parent_edges().size() == 1u);
    reverseDSF(root->get_parent_edges()[0]->parent_node());

    std::cout << "== Final search_num = " << search_num << std::endl;
    return subgraph;
}

// This algorithm is from Maxsim
// 1.	Perform reverse DFS (in the upward direction) from the ReadValue node and store all the nodes discoverd in this DFS pass in the candidates set. This will const up to O(v)
// 2.	Perform forward DFS (in the graph direction) from the ReadValue node storing all the discovered nodes in the reachable set. This will const up to O(v - p) where p is the size of the candidates
// 3.	Perform forward DFS from each member of the candidates set and check if it discovers a node from the reachable set or an output, if so, such a node and all the nodes on the DFS  Path should be stored to the reachable set, including the candidate itself. This will const O(v - p - r), where r - is the size of the reachable set. If such a DFS path doesn't meet a "reachable" node or an "output", such a node then transfered to `subgraph_nodes` set.
// In the end candidates is empty, and `subgraph_nodes` contains only nodes whose DFS subtrees end up in the ReadValue. You can see that the total complexity is O(v). What do you think?
// 
// Shortage: it's hard to confirm who is input of subgraph.(need to search again?)
GraphPtr find_init_graph_2(GraphPtr graph, NodePtr root) {
    GraphPtr subgraph = createGraph("subgraph");

    int search_num = 0;
    std::unordered_set<NodePtr> candidatesNodes;
    std::unordered_set<NodePtr> reachableNodes;

    bool is_subgraph_node = true;
    std::function<void(NodePtr)> candiatesDSF = [&](NodePtr node){
        search_num++;
        DEBUG_LOG("  == candiatesDSF: " << node << ", search_num=" << search_num);

        // Found node in reachableNodes
        if (reachableNodes.find(node) != reachableNodes.end()) {
            is_subgraph_node = false;
            return;
        }

        if (node->get_type() == NodeType::Result) {
            is_subgraph_node = false;
            return;
        }

        for (auto son_edge : node->get_son_edges()) {
            candiatesDSF(son_edge->son_node());
        }
    };

    std::function<void(NodePtr)> reverseDSF = [&](NodePtr node) {
        search_num ++;
        DEBUG_LOG("== reverseDSF : " << node << ", search_num = " << search_num);
        candidatesNodes.insert(node);
        for (auto parent_edges : node->get_parent_edges()) {
            reverseDSF(parent_edges->parent_node());
        }
    };

    std::function<void(NodePtr)> forwardDSF = [&](NodePtr node){
        search_num ++;
        DEBUG_LOG("== forwardDSF : " << node << ", search_num = " << search_num);
        reachableNodes.insert(node);

        for (auto son_edge : node->get_son_edges()) {
            forwardDSF(son_edge->son_node());
        }
    };

    // Step 1, get candidatesNodes
    assert(root->get_parent_edges().size() == 1u);
    reverseDSF(root->get_parent_edges()[0]->parent_node());

    // Step 2, get reachableNodes
    forwardDSF(root);

    // Step 3,
    for (auto cand : candidatesNodes) {
        is_subgraph_node = true;
        candiatesDSF(cand);
        if (is_subgraph_node) {
            subgraph->add_node(cand);
        }
    }

    std::cout << "== candidatesNodes = ";
    for (auto cand : candidatesNodes) {
        std::cout << cand << ", ";
    }
    std::cout << std::endl;

    std::cout << "== reachableNodes = ";
    for (auto r : reachableNodes) {
        std::cout << r << ", ";
    }
    std::cout << std::endl;

    std::cout << "== Final search_num = " << search_num << std::endl;
    return subgraph;
}
