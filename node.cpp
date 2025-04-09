#include "node.h"

Node::Node(int id, double x, double y, const QString &name,const Type &type)
    : m_id(id), m_x(x), m_y(y), m_name(name),m_Type(type) {}

int Node::getId() const{
    return m_id;
}

void Node::setId(int id){
    m_id=id;
}

double Node::getX() const { return m_x; }
void Node::setX(double x) { m_x = x; }

double Node::getY() const { return m_y; }
void Node::setY(double y) { m_y = y; }

QString Node::getName() const { return m_name; }
void Node::setName(const QString &name) { m_name = name; }


Type Node::getType() const { return m_Type; }
void Node::setType(const Type &type) { m_Type = type;}
