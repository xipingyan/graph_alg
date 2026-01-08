#include <unordered_map>

#include "../utils/graph.hpp"
#include "../utils/profiler.hpp"

/* =================Graph===================
 *              0
 *              |
 *              1
 *             / \
 *            2   3
 *             \ /
 *              4
 * 
 * In this simple graph, 2 and 3 will be executed in parallel.
 */
GraphPtr generate_test_graph()
{
    PROFILE(P, __FUNCTION__);
    auto graph = createGraph("graph_dag");

    std::vector<NodePtr> vecNodes;
    std::unordered_map<int, NodeType> types;
    types[0] = NodeType::Parameter;
    types[4] = NodeType::Result;
    {
        for (int i = 0; i < 5; i++)
        {
            PROFILE_ARGS(ien, "init execute node.", {{"idx", std::to_string(i)}});
            NodePtr node = std::make_shared<Node>(types[i] ? types[i] : NodeType::Execution, std::to_string(i));

            node->set_execute_time(i);
            graph->add_node(node);
        }
    }

    {
        PROFILE(P, __FUNCTION__);
        CreateEdge(graph->get_node(0), graph->get_node(1));
        CreateEdge(graph->get_node(1), graph->get_node(2));
        CreateEdge(graph->get_node(1), graph->get_node(3));
        CreateEdge(graph->get_node(2), graph->get_node(4));
        CreateEdge(graph->get_node(3), graph->get_node(4));
    }

    graph->show_in_cmd();
    return graph;
}

int main(int argc, char **argv)
{
    PROFILE(m, "main");
    auto graph = generate_test_graph();

    for (size_t i = 0; i < 3; i++)
    {
        PROFILE_ARGS(e, "execute", {{"i", std::to_string(i)}});
        graph->execute();
    }
    return EXIT_SUCCESS;
}