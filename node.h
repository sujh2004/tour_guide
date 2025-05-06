#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QPen>
#include <QTimer>
#include <QVector>
enum Type {
    BUILDING,  // 建筑
    INTERSECTION,  // 路口
    TOLIET,//厕所
    RESTERANT//饭店
};

class Node
{
public:

    Node(int id = 0, double x = 0.0, double y = 0.0,
         const QString &name = "", const Type &type = BUILDING,
         double rating = 0.0, bool isFavorite = false,
         const QString &description = "", const QString &imagePath = "");


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

    double getRating() const;
    void setRating(double rating);

    bool getIsFavorite() const;
    void setIsFavorite(bool favorite);

    QString getDescription() const;
    void setDescription(const QString &desc);

    QString getImagePath() const;
    void setImagePath(const QString &path);

    Node *getNext() const;
    void setNext(Node *next);
private:
    int m_id;
    double m_x;
    double m_y;
    QString m_name;
    Type m_Type;
    double m_rating;
    bool m_isFavorite;
    QString m_description;
    QString m_imagePath;
    Node *next;//for NodeList
};


#endif // NODE_H
