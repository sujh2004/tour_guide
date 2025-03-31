#ifndef MAP_H
#define MAP_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

class Map : public QWidget
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = nullptr);
    void setMapImage(const QPixmap &image);
    void drawRoute(const QPoint &start, const QPoint &end);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QPixmap mapImage;
    QPoint startPoint;
    QPoint endPoint;
};

#endif // MAP_H
