#include <algorithm>
#include <cassert>

#include "private.hpp"

GraphPtr find_init_graph(GraphPtr graph, NodePtr root) {
    GraphPtr subgraph = createGraph("subgraph");

    std::unordered_set<NodePtr> flag;
    bool found_final_successor = false;
    std::function<void(NodePtr, NodePtr)> check_succesor = [&](NodePtr node, NodePtr final_succesor){
        std::cout << "  == find_succesor: " << node << std::endl;
        for (auto son_edge : node->get_son_edges()) {
            if (son_edge->son_node() == final_succesor) {
                found_final_successor = true;
                return;
            }
            check_succesor(son_edge->son_node(), final_succesor);
        }
    };

    std::function<void(NodePtr)> dsf = [&](NodePtr node) {
        subgraph->add_node(node);

        if (node->get_type() == NodeType::Parameter) {
            return;
        }

        std::cout << "== Start check sucessor : " << node << std::endl;
        found_final_successor = false;
        check_succesor(node, root);
        std::cout << "--> found_final_successor : " << found_final_successor << std::endl;
        if (!found_final_successor) {
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
