#include "node.h"

Node::Node(int id, double x, double y, const QString &name, const Type &type,
           double rating, bool isFavorite, const QString &description, const QString &imagePath)
    : m_id(id), m_x(x), m_y(y), m_name(name), m_Type(type),
    m_rating(rating), m_isFavorite(isFavorite),
    m_description(description), m_imagePath(imagePath),
    next(nullptr) {}


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

double Node::getRating() const {return m_rating;}
void Node::setRating(double rating){m_rating = rating;}

QString Node::getDescription() const {return m_description;}
void Node::setDescription(const QString &desc){ m_description = desc;}

QString Node::getImagePath() const { return m_imagePath;}
void Node::setImagePath(const QString &path) { m_imagePath = path;}

bool Node::getIsFavorite() const {return m_isFavorite;}
void Node::setIsFavorite(bool favorite){m_isFavorite = favorite;}

Node* Node::getNext() const{return next;}
void  Node::setNext(Node* next){this->next = next;}
