#ifndef DEVICESSEARCHWINDOW_H
#define DEVICESSEARCHWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QGraphicsScene>
#include <vector>
#include "node.h" // 引入 node.h 以便使用设施类型和节点

class DeviceSearchWindow : public QDialog {
    Q_OBJECT

public:
    DeviceSearchWindow(const std::vector<Node> &nodes, QGraphicsScene *scene, QWidget *parent = nullptr);
    ~DeviceSearchWindow();

private slots:
    void onSearchClicked();
    void onClearClicked();
    void clearHighlightedDevices();  // 添加清除高亮的函数

private:
    QComboBox *deviceTypeComboBox;  // 用来选择目标设施类型（饭店、厕所）
    QLineEdit *currentPosLineEdit;  // 当前所在位置输入框
    QLineEdit *maxDistanceLineEdit; // 最大距离输入框
    QPushButton *searchButton;      // 搜索按钮
    QPushButton *clearButton;       // 清除按钮

    std::vector<Node> nodes;        // 存储节点
    QGraphicsScene *scene;          // 存储场景
    std::vector<QGraphicsEllipseItem*> highlightedItems;  // 存储已高亮显示的设施项
    void findNearbyDevices(double x, double y, double maxDist, Type deviceType);
    void highlightDevices(const std::vector<int>& devices);
};

#endif // DEVICESSEARCHWINDOW_H
