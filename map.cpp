#include "map.h"
#include <QPainter>
#include <QPen>
#include <QMessageBox>

Map::Map(QWidget *parent)
    : QWidget(parent)
{
    // 初始化地图图像为空
    mapImage = QPixmap();
}

void Map::setMapImage(const QPixmap &image)
{
    mapImage = image;
    update();  // 更新界面
}

void Map::drawRoute(const QPoint &start, const QPoint &end)
{
    startPoint = start;
    endPoint = end;
    update();  // 更新地图界面，绘制路径
}

void Map::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 如果地图图像存在，绘制地图
    if (!mapImage.isNull()) {
        painter.drawPixmap(0, 0, mapImage);
    }

    // 绘制路径
    if (!startPoint.isNull() && !endPoint.isNull()) {
        QPen pen(Qt::red);
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawLine(startPoint, endPoint);  // 绘制起点到终点的路径
    }
}

void Map::mousePressEvent(QMouseEvent *event)
{
    // 可以根据需要处理鼠标点击事件，进行地图交互
    // 例如，获取用户点击的坐标
    QPoint clickPos = event->pos();
    qDebug() << "User clicked at:" << clickPos;
}
