#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include <vector>
#include "node.h"
#include "edge.h"

class RoutePlanner {
public:
    RoutePlanner(const std::vector<Node> &nodes, const std::vector<Edge> &edges);

    // 计算最短路径
    std::vector<int> calculateShortestPath(int startId, int endId);

private:
    std::vector<Node> m_nodes;
    std::vector<Edge> m_edges;
};

#endif // ROUTEPLANNER_H
