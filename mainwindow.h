#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QInputDialog>
#include <QGraphicsItemGroup> // 用于存储路径图形项
#include <vector>
#include "node.h"
#include "edge.h"
#include "routeplanner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPlanRouteClicked();

private:
    QWidget *centralWidget;   // 中央部件
    QGraphicsView *graphicsView; // 显示地图的视图
    QGraphicsScene *scene;    // 场景，管理所有图形项
    QPushButton *planRouteButton; // 规划路线的按钮

    std::vector<Node> nodes;  // 存储节点
    std::vector<Edge> edges;  // 存储边
    RoutePlanner *planner;    // 路径规划器
    int startPoint = -1;  // 起点
    int endPoint = -1;    // 终点

    QGraphicsItemGroup *currentPathItemGroup;  // 存储当前高亮路径的图形项组
    void setupUI();  // 设置界面
    void initData(); // 初始化数据（节点、边）
    void drawMap();  // 绘制地图
    void onNodeButtonClicked(int nodeId);
    void planRoute(int startId, int endId);
};
#endif // MAINWINDOW_H
