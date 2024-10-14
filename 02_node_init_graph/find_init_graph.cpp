#include <algorithm>
#include <cassert>

#include "private.hpp"

GraphPtr find_init_graph(GraphPtr graph, NodePtr root) {
    GraphPtr subgraph = createGraph("subgraph");

    int find_num = 0;
    std::unordered_set<NodePtr> flag;
    bool found_result = true; // Find result or out of max depth.
    std::function<void(NodePtr, NodePtr)> check_succesor = [&](NodePtr node, NodePtr final_succesor){
        if (flag.find(node) != flag.end()) {
            DEBUG_LOG("  ** " << node << " is flagged, skipped.");
            return;
        }

        find_num++;
        DEBUG_LOG("  == check_succesor: " << node << ", find_num=" << find_num);

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

    std::function<void(NodePtr)> dsf = [&](NodePtr node) {
        subgraph->add_node(node);

        // if (node->get_type() == NodeType::Parameter) {
        //     return;
        // }

        // find_num = 0;
        DEBUG_LOG("== Start check sucessor : " << node);
        found_result = false;
        check_succesor(node, root);
        flag.insert(node);
        DEBUG_LOG("--> found_result : " << found_result);
        if (found_result) {
            return;
        }

        for (auto parent_edges : node->get_parent_edges()) {
            dsf(parent_edges->parent_node());
        }
    };

    assert(root->get_parent_edges().size() == 1u);
    dsf(root->get_parent_edges()[0]->parent_node());

    return subgraph;
}
