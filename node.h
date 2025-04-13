#ifndef NODE_H
#define NODE_H

#include <QString>

enum Type {
    BUILDING,  // 建筑
    INTERSECTION,  // 路口
    TOLIET,//厕所
    RESTERANT//饭店
};

class Node
{
public:

    Node(int id = 0, double x = 0.0, double y = 0.0, const QString &name = "",const Type &type=BUILDING);

    int getId() const;
    void setId(int id);

    double getX() const;
    void setX(double x);

    double getY() const;
    void setY(double y);

    QString getName() const;
    void setName(const QString &name);

    Type getType() const;
    void setType(const Type &type);

private:
    int m_id;
    double m_x;
    double m_y;
    QString m_name;
    Type m_Type;
};

#endif // NODE_H//hello
