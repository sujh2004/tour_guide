// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QPainter>
#include "map.h"  // 引入 Map 类


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showRouteInputDialog();

private:
    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QPushButton *button;
    Map *map;  // Map 类对象，用于显示地图和绘制路径

    // Add the function declaration
    void drawRouteOnMap(const QString &startPoint, const QString &endPoint);
    void changeImage();
};

#endif // MAINWINDOW_H
