#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QGraphicsPixmapItem>
#include <QVector>
#include <QObject>
#include "node.h"

class QGraphicsScene;

class PlayerController : public QObject {
    Q_OBJECT
public:
    PlayerController(QGraphicsScene *scene, const std::vector<Node> &nodes, QObject *parent = nullptr);
    ~PlayerController();

    void setPath(const std::vector<int> &path);   // 设置路径并初始化人物
    void moveForward();                           // 向前移动
    void moveBackward();                          // 向后移动
    void reset();                                 // 重置角色

private:
    void moveTo(int index);                       // 移动到路径中的某个节点

    QGraphicsScene *scene;
    QGraphicsPixmapItem *playerItem;
    const std::vector<Node> &nodes;
    std::vector<int> currentPath;
    QVector<QGraphicsLineItem*> *pathLines;
    int currentIndex;

public:
    void setPathLines(QVector<QGraphicsLineItem*> *lines);  // 设置路径线引用
};

#endif // PLAYERCONTROLLER_H
