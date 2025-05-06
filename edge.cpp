#include "edge.h"

Edge::Edge(int from, int to, double distance)
    : m_from(from), m_to(to), m_distance(distance) {}

int Edge::getFrom() const { return m_from; }
void Edge::setFrom(int from) { m_from = from; }

int Edge::getTo() const { return m_to; }
void Edge::setTo(int to) { m_to = to; }

double Edge::getDistance() const { return m_distance; }
void Edge::setDistance(double distance) { m_distance = distance; }
