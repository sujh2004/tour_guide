#include "routeplanner.h"
#include <queue>
#include <limits>
#include <algorithm>

RoutePlanner::RoutePlanner(const std::vector<Node> &nodes, const std::vector<Edge> &edges)
    : m_nodes(nodes), m_edges(edges) {}

std::vector<int> RoutePlanner::calculateShortestPath(int startId, int endId) {
    std::vector<double> dist(m_nodes.size(), std::numeric_limits<double>::infinity());
    std::vector<int> prev(m_nodes.size(), -1);
    std::vector<bool> visited(m_nodes.size(), false);

    dist[startId] = 0;
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;
    pq.push({0, startId});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (const auto &edge : m_edges) {
            if (edge.getFrom() == u) {
                int v = edge.getTo();
                double newDist = dist[u] + edge.getDistance();
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                    pq.push({newDist, v});
                }
            }
        }
    }

    std::vector<int> path;
    for (int at = endId; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    return path;
}
