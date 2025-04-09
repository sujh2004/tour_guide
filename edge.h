#ifndef EDGE_H
#define EDGE_H

class Edge {
public:
    Edge(int from, int to, double distance);

    int getFrom() const;
    void setFrom(int from);

    int getTo() const;
    void setTo(int to);

    double getDistance() const;
    void setDistance(double distance);

private:
    int m_from;
    int m_to;
    double m_distance;
};

#endif // EDGE_H
