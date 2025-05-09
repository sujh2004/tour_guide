#include "playercontroller.h"
#include <QGraphicsScene>
#include <QPen>

PlayerController::PlayerController(QGraphicsScene *scene, const std::vector<Node> &nodes, QObject *parent)
    : QObject(parent), scene(scene), nodes(nodes), playerItem(nullptr), currentIndex(0), pathLines(nullptr) {}

PlayerController::~PlayerController() {
    reset();
}

void PlayerController::setPath(const std::vector<int> &path) {

    reset();
    currentPath = path;
    currentIndex = 0;

    if (pathLines) {
        for (auto *line : *pathLines) {
            line->setPen(QPen(QColor("#E1D279"), 2));  // 重置颜色
        }
    }
    if (!path.empty()) {
        const Node &startNode = nodes[path[0]];
        QPixmap pix(":/images/src/character.png");  // 替换为你的人物图片路径
        playerItem = scene->addPixmap(pix.scaled(30, 30));
        playerItem->setPos(startNode.getX() - 15, startNode.getY() - 15);
        playerItem->setZValue(10);
    }
}

void PlayerController::setPathLines(QVector<QGraphicsLineItem*> *lines) {
    pathLines = lines;
}

void PlayerController::moveForward() {
    if (currentIndex + 1 < currentPath.size()) {
        ++currentIndex;
        moveTo(currentIndex);

        if (pathLines && currentIndex - 1 < pathLines->size()) {
            (*pathLines)[currentIndex - 1]->setPen(QPen(Qt::gray, 1));
        }
    }

    if (currentIndex == currentPath.size() - 1) {
        emit reachedDestination(currentPath.back());  /*** 传终点 ***/
    }

}


void PlayerController::moveBackward() {
    if (currentIndex > 0) {
        --currentIndex;
        moveTo(currentIndex);
    }
}

void PlayerController::moveTo(int index) {
    const Node &node = nodes[currentPath[index]];
    playerItem->setPos(node.getX() - 15, node.getY() - 15);
}

void PlayerController::reset() {
    if (playerItem) {
        scene->removeItem(playerItem);
        delete playerItem;
        playerItem = nullptr;
    }
    currentPath.clear();
    currentIndex = 0;
}

