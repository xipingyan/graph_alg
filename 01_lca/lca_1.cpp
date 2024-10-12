#include <algorithm>

#include "../utils/graph.hpp"
#include "private.hpp"

// Refer: https://cp-algorithms.com/graph/lca.html
// 
#define SHOW_VERTICES_HEIGHT 0

struct LCA {
    std::vector<NodePtr> vertices;
    std::vector<int> height;

    LCA(GraphPtr graph, size_t root_idx = 0)
    {
        // Depth first search
        dfs(graph->get_node(0), 1);
        
#if SHOW_VERTICES_HEIGHT
        std::cout << "\n== =========================" << std::endl;
        std::cout << "== vertices:" << std::endl;
        for (auto v : vertices) {
            std::cout << v << ", ";
        }
        std::cout << "\n== height:" << std::endl;
        for (auto h : height) {
            std::cout << h << ", ";
        }
        std::cout << "\n== =========================" << std::endl;
#endif
    }

    // Depth first search
    void dfs(NodePtr node, int h)
    {
        vertices.emplace_back(node);
        height.emplace_back(h);
        for (auto son : node->get_son())
        {
            dfs(son, h + 1);
            vertices.emplace_back(node);
            height.emplace_back(h);
        }
    }

    NodePtr lca(NodePtr u, NodePtr v) {
        auto it_u = std::find(vertices.begin(), vertices.end(), u);
        auto it_v = std::find(vertices.begin(), vertices.end(), v);
        if (it_u != vertices.end() && it_v != vertices.end()) {
            auto diff_u = it_u - vertices.begin();
            auto diff_v = it_v - vertices.begin();
            size_t left_id, right_id;
            if (diff_u < diff_v)
            {
                left_id = diff_u;
                right_id = diff_v;
            }
            else
            {

                left_id = diff_v;
                right_id = diff_u;
            }
 
            int minval = std::numeric_limits<int>::max();
            size_t min_idx = 0;
            for (size_t i = left_id; i <= right_id; i++)
            {
                if (height[i] < minval)
                {
                    minval = height[i];
                    min_idx = i;
                }
            }

            return vertices[min_idx];
        }

        return nullptr;
    }
};

NodePtr lca_1(GraphPtr graph, NodePtr a, NodePtr b)
{
    return LCA(graph, 0).lca(a, b);
}
