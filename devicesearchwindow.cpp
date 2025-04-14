#include "devicesearchwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <cmath>  // 用于计算距离
#include<QGraphicsEllipseItem>

DeviceSearchWindow::DeviceSearchWindow(const std::vector<Node> &nodes, QGraphicsScene *scene, QWidget *parent)
    : QDialog(parent), nodes(nodes), scene(scene) {

    // 设置背景色和窗口样式
    this->setStyleSheet("background-color: #F8F0E3;"); // 浅黄色背景

    QVBoxLayout *layout = new QVBoxLayout(this);

    // 创建控件
    deviceTypeComboBox = new QComboBox(this);
    deviceTypeComboBox->addItem("饭店");
    deviceTypeComboBox->addItem("厕所");

    currentPosLineEdit = new QLineEdit(this);
    maxDistanceLineEdit = new QLineEdit(this);

    // 设置按钮样式
    searchButton = new QPushButton("搜索", this);
    searchButton->setStyleSheet("background-color: #C7A47A; color: white; font-family: 'Courier New'; border-radius: 5px;");
    clearButton = new QPushButton("清除高亮", this);
    clearButton->setStyleSheet("background-color: #C7A47A; color: white; font-family: 'Courier New'; border-radius: 5px;");

    layout->addWidget(new QLabel("选择您想要的服务（卫生间/饭店）："));
    layout->addWidget(deviceTypeComboBox);
    layout->addWidget(new QLabel("输入当前位置（如午门，皇极殿等）："));
    layout->addWidget(currentPosLineEdit);
    layout->addWidget(new QLabel("输入您可以接受的最远距离："));
    layout->addWidget(maxDistanceLineEdit);
    layout->addWidget(searchButton);
    layout->addWidget(clearButton);

    connect(searchButton, &QPushButton::clicked, this, &DeviceSearchWindow::onSearchClicked);
    connect(clearButton, &QPushButton::clicked, this, &DeviceSearchWindow::onClearClicked);

    // 当弹窗关闭时自动清除标亮
    connect(this, &QDialog::finished, this, &DeviceSearchWindow::clearHighlightedDevices);
}

DeviceSearchWindow::~DeviceSearchWindow() {
    clearHighlightedDevices();  // 在弹窗销毁时清除高亮显示的设备
}

void DeviceSearchWindow::onSearchClicked() {
    bool okDist;
    double maxDist = maxDistanceLineEdit->text().toDouble(&okDist);

    if (!okDist || currentPosLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请确保输入有效的当前位置和最远距离");
        return;
    }

    // 获取当前位置的节点 ID
    QString currentPos = currentPosLineEdit->text();
    int startId = -1;
    for (const auto &node : nodes) {
        if (node.getName() == currentPos) {
            startId = node.getId();
            break;
        }
    }

    if (startId == -1) {
        QMessageBox::warning(this, "错误", "未找到该位置，请检查位置名称");
        return;
    }

    // 根据选择的设施类型进行查询
    QString deviceType = deviceTypeComboBox->currentText();
    Type type = (deviceType == "饭店") ? RESTERANT : TOLIET;

    // 清除之前的标亮
    clearHighlightedDevices();

    // 查找附近的设施并高亮
    findNearbyDevices(nodes[startId].getX(), nodes[startId].getY(), maxDist, type);
}

void DeviceSearchWindow::onClearClicked() {
    clearHighlightedDevices();  // 清除已高亮的设施
}

void DeviceSearchWindow::findNearbyDevices(double x, double y, double maxDist, Type deviceType) {
    std::vector<int> nearbyDevices;
    for (const auto &node : nodes) {
        if (node.getType() == deviceType) {
            double dist = std::sqrt(std::pow(node.getX() - x, 2) + std::pow(node.getY() - y, 2));
            if (dist <= maxDist) {
                nearbyDevices.push_back(node.getId());
            }
        }
    }

    // 高亮显示符合条件的设备
    highlightDevices(nearbyDevices);
}

void DeviceSearchWindow::highlightDevices(const std::vector<int>& devices) {
    QPen highlightPen(Qt::green, 2);
    for (int id : devices) {
        const Node &node = nodes[id];
        QGraphicsEllipseItem *highlightItem = new QGraphicsEllipseItem(node.getX() - 5, node.getY() - 5, 10, 10);
        highlightItem->setPen(highlightPen);
        highlightItem->setBrush(QBrush(Qt::green));
        scene->addItem(highlightItem);

        highlightedItems.push_back(highlightItem);  // 保存高亮设备
    }
}

void DeviceSearchWindow::clearHighlightedDevices() {
    for (auto item : highlightedItems) {
        scene->removeItem(item);  // 从场景中移除高亮设备
        delete item;  // 删除图形项
    }
    highlightedItems.clear();  // 清空已高亮设备的列表
}
