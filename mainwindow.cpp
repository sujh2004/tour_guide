#include "mainwindow.h"
#include <QVBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup> // 用于存储路径的图形项
#include <QMouseEvent>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    planRouteButton(new QPushButton("规划路线", this)),
    planner(nullptr),
    currentPathItemGroup(nullptr) // 初始化当前路径项组为空
{
    resize(650,950);  // 设置初始窗口大小为 800x600
    setCentralWidget(centralWidget);
    setupUI(); // 设置界面
    initData(); // 初始化数据
    drawMap();

    // 创建路径规划器
    planner = new RoutePlanner(nodes, edges);

}

MainWindow::~MainWindow() {
    delete planner;
}

void MainWindow::setupUI() {
    // 设置中央部件的布局
    QVBoxLayout *layout = new QVBoxLayout;
    centralWidget->setLayout(layout);

    // 设置 QGraphicsView
    graphicsView->setScene(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(graphicsView);
    planRouteButton->move(100, 150);  // 设置按钮位置为 (x=100, y=150)
    planRouteButton->setStyleSheet("background: transparent; border: none; color: transparent;");
    // 添加按钮
    //layout->addWidget(planRouteButton);
}

void MainWindow::initData() {
    // 示例：创建一些节点
    nodes.push_back(Node(0, 75, 235, "角楼",BUILDING));
    nodes.push_back(Node(1, 285, 235, "神武门",BUILDING));
    nodes.push_back(Node(2, 504, 235, "角楼",BUILDING));
    nodes.push_back(Node(3, 285, 310, "御花园",BUILDING));
    nodes.push_back(Node(4, 505, 310, "乐寿堂",BUILDING));
    nodes.push_back(Node(5, 285, 370, "坤宁宫",BUILDING));
    nodes.push_back(Node(6, 285, 445, "乾清宫",BUILDING));
    nodes.push_back(Node(7, 375, 445, "景仁宫",BUILDING));
    nodes.push_back(Node(8, 505, 425, "宁寿宫",BUILDING));
    nodes.push_back(Node(9, 110, 390, "寿安宫",BUILDING));
    nodes.push_back(Node(10, 320, 470, "斋宫",BUILDING));
    nodes.push_back(Node(11, 235, 470, "养心殿",BUILDING));
    nodes.push_back(Node(12, 80, 480, "寿康宫",BUILDING));
    nodes.push_back(Node(13, 237, 400, "翊坤宫",BUILDING));
    nodes.push_back(Node(14, 505, 480, "皇极殿",BUILDING));
    nodes.push_back(Node(15, 153, 420, "雨花阁",BUILDING));
    nodes.push_back(Node(16, 155, 565, "慈宁宫",BUILDING));
    nodes.push_back(Node(17, 285, 573, "保和殿",BUILDING));
    nodes.push_back(Node(18, 285, 611, "中和殿",BUILDING));
    nodes.push_back(Node(19, 370, 600, "剑亭",BUILDING));
    nodes.push_back(Node(20, 415, 655, "文渊阁",BUILDING));
    nodes.push_back(Node(21, 505, 660, "南三所",BUILDING));
    nodes.push_back(Node(22, 285, 670, "太和殿",BUILDING));
    nodes.push_back(Node(23, 285, 745, "太和门",BUILDING));
    nodes.push_back(Node(24, 415, 710, "文华殿",BUILDING));
    nodes.push_back(Node(25, 155, 755, "武英殿",BUILDING));
    nodes.push_back(Node(26, 285, 830, "午门",BUILDING));
    nodes.push_back(Node(27, 560, 830, "东华门",BUILDING));
    nodes.push_back(Node(28, 560, 865, "角楼",BUILDING));
    nodes.push_back(Node(29, 55, 865, "角楼",BUILDING));
    nodes.push_back(Node(30, 110, 235,"路口1",INTERSECTION));
    nodes.push_back(Node(31, 153, 235, "路口2",INTERSECTION));
    nodes.push_back(Node(32, 218, 235, "卫生间",TOLIET));
    nodes.push_back(Node(33, 432, 235, "青禾宴",RESTERANT));
    nodes.push_back(Node(34, 153, 300, "路口5",INTERSECTION));
    nodes.push_back(Node(35, 110, 300, "路口6",INTERSECTION));
    nodes.push_back(Node(36, 153, 340, "路口7",INTERSECTION));
    nodes.push_back(Node(37, 110, 340, "路口8",INTERSECTION));
    nodes.push_back(Node(38, 153, 480, "路口9",INTERSECTION));
    nodes.push_back(Node(39, 153, 455, "路口10",INTERSECTION));
    nodes.push_back(Node(40, 200, 455, "路口11",INTERSECTION));
    nodes.push_back(Node(41, 200, 340, "路口12",INTERSECTION));
    nodes.push_back(Node(42, 215, 310, "路口13",INTERSECTION));
    nodes.push_back(Node(43, 375, 310, "路口14",INTERSECTION));
    nodes.push_back(Node(44, 432, 310, "路口15",INTERSECTION));
    nodes.push_back(Node(45, 375, 470, "路口32",INTERSECTION));
    nodes.push_back(Node(46, 432, 470, "路口17",INTERSECTION));
    nodes.push_back(Node(47, 237, 340, "路口18",INTERSECTION));
    nodes.push_back(Node(48, 285, 470, "路口19",INTERSECTION));
    nodes.push_back(Node(49, 415, 600, "路口20",INTERSECTION));
    nodes.push_back(Node(50, 170, 611, "路口21",INTERSECTION));
    nodes.push_back(Node(51, 170, 670, "食客人间",RESTERANT));
    nodes.push_back(Node(52, 370, 655, "路口23",INTERSECTION));
    nodes.push_back(Node(53, 415, 830, "路口24",INTERSECTION));
    nodes.push_back(Node(54, 505, 865, "路口25",INTERSECTION));
    nodes.push_back(Node(55, 200, 830, "路口26",INTERSECTION));
    nodes.push_back(Node(56, 200, 865, "路口27",INTERSECTION));
    nodes.push_back(Node(57, 285, 865, "路口28",INTERSECTION));
    nodes.push_back(Node(58, 155, 830, "路口29",INTERSECTION));
    nodes.push_back(Node(59, 505, 830, "路口30",INTERSECTION));
    nodes.push_back(Node(60, 215, 340, "路口31",INTERSECTION));
    nodes.push_back(Node(61, 415, 470, "卫生间",TOLIET));
    nodes.push_back(Node(62, 375, 390, "食野集",RESTERANT));
    nodes.push_back(Node(63, 415, 530, "松间小叙",RESTERANT));
    nodes.push_back(Node(64, 142, 830, "卫生间",TOLIET));
    nodes.push_back(Node(65, 415, 775, "卫生间",TOLIET));


    // 示例：添加一些边
    edges.push_back(Edge(0, 30, 35.0));
    edges.push_back(Edge(30, 0, 35.0));
    edges.push_back(Edge(30, 31, 43.0));
    edges.push_back(Edge(31, 30, 43.0));
    edges.push_back(Edge(31, 32, 65.0));
    edges.push_back(Edge(32, 31, 65.0));
    edges.push_back(Edge(32, 1, 66.0));
    edges.push_back(Edge(1, 32, 66.0));
    edges.push_back(Edge(1, 33, 148.0));
    edges.push_back(Edge(33, 1, 148.0));
    edges.push_back(Edge(33, 2, 72.0));
    edges.push_back(Edge(2, 33, 72.0));
    edges.push_back(Edge(30, 35, 65.0));
    edges.push_back(Edge(35, 30, 65.0));
    edges.push_back(Edge(34, 31, 65.0));
    edges.push_back(Edge(31, 34, 65.0));
    edges.push_back(Edge(35, 34, 43.0));
    edges.push_back(Edge(34, 35, 43.0));
    edges.push_back(Edge(35, 37, 40.0));
    edges.push_back(Edge(37, 35, 40.0));
    edges.push_back(Edge(36, 37, 43.0));
    edges.push_back(Edge(37, 36, 43.0));
    edges.push_back(Edge(36, 34, 40.0));
    edges.push_back(Edge(34, 36, 40.0));
    edges.push_back(Edge(37, 9, 50.0));
    edges.push_back(Edge(9, 37, 50.0));
    edges.push_back(Edge(15, 36, 80.0));
    edges.push_back(Edge(36, 15, 80.0));
    edges.push_back(Edge(15, 39, 35.0));
    edges.push_back(Edge(39, 15, 35.0));
    edges.push_back(Edge(39, 40, 47.0));
    edges.push_back(Edge(40, 39, 47.0));
    edges.push_back(Edge(40, 41, 115.0));
    edges.push_back(Edge(41, 40, 115.0));
    edges.push_back(Edge(41, 47, 37.0));
    edges.push_back(Edge(47, 41, 37.0));
    edges.push_back(Edge(41, 60, 15.0));
    edges.push_back(Edge(60, 41, 15.0));
    edges.push_back(Edge(60, 42, 30.0));
    edges.push_back(Edge(42, 60, 30.0));
    edges.push_back(Edge(42, 32, 75.0));
    edges.push_back(Edge(32, 42, 75.0));
    edges.push_back(Edge(3, 42, 70.0));
    edges.push_back(Edge(42, 3, 70.0));
    edges.push_back(Edge(3, 1, 75.0));
    edges.push_back(Edge(1, 3, 75.0));
    edges.push_back(Edge(3, 43, 90.0));
    edges.push_back(Edge(43, 3, 90.0));
    edges.push_back(Edge(43, 44, 57.0));
    edges.push_back(Edge(44, 43, 57.0));
    edges.push_back(Edge(44, 33, 75.0));
    edges.push_back(Edge(33, 44, 75.0));
    edges.push_back(Edge(2, 4, 75.0));
    edges.push_back(Edge(4, 2, 75.0));
    edges.push_back(Edge(44, 4, 73.0));
    edges.push_back(Edge(4, 44, 73.0));
    edges.push_back(Edge(5, 3, 60.0));
    edges.push_back(Edge(3, 5, 60.0));
    edges.push_back(Edge(6, 5, 75.0));
    edges.push_back(Edge(5, 6, 75.0));
    edges.push_back(Edge(62, 43, 80.0));
    edges.push_back(Edge(43, 62, 80.0));
    edges.push_back(Edge(7, 62, 55.0));
    edges.push_back(Edge(62, 7, 55.0));
    edges.push_back(Edge(47, 13, 60.0));
    edges.push_back(Edge(13, 47, 60.0));
    edges.push_back(Edge(13, 11, 70.0));
    edges.push_back(Edge(11, 13, 70.0));
    edges.push_back(Edge(48, 11, 50.0));
    edges.push_back(Edge(11, 48, 50.0));
    edges.push_back(Edge(6, 48, 25.0));
    edges.push_back(Edge(48, 6, 25.0));
    edges.push_back(Edge(48, 10, 35.0));
    edges.push_back(Edge(10, 48, 35.0));
    edges.push_back(Edge(45, 10, 55.0));
    edges.push_back(Edge(10, 45, 55.0));
    edges.push_back(Edge(45, 7, 25.0));
    edges.push_back(Edge(7, 45, 25.0));
    edges.push_back(Edge(44, 46, 160.0));
    edges.push_back(Edge(46, 44, 160.0));
    edges.push_back(Edge(4, 8, 115.0));
    edges.push_back(Edge(8, 4, 115.0));
    edges.push_back(Edge(8, 14, 55.0));
    edges.push_back(Edge(14, 8, 55.0));
    edges.push_back(Edge(48, 17, 103.0));
    edges.push_back(Edge(17, 48, 103.0));
    edges.push_back(Edge(17, 18, 38.0));
    edges.push_back(Edge(18, 17, 38.0));
    edges.push_back(Edge(18, 50, 115.0));
    edges.push_back(Edge(50, 18, 115.0));
    edges.push_back(Edge(51, 22, 115.0));
    edges.push_back(Edge(22, 51, 115.0));
    edges.push_back(Edge(16, 50, 61.0));
    edges.push_back(Edge(50, 16, 61.0));
    edges.push_back(Edge(16, 38, 85.0));
    edges.push_back(Edge(38, 16, 85.0));
    edges.push_back(Edge(12, 38, 73.0));
    edges.push_back(Edge(38, 12, 73.0));
    edges.push_back(Edge(50, 51, 60.0));
    edges.push_back(Edge(51, 50, 60.0));
    edges.push_back(Edge(18, 22, 60.0));
    edges.push_back(Edge(22, 18, 60.0));
    edges.push_back(Edge(45, 61, 40.0));
    edges.push_back(Edge(61, 45, 40.0));
    edges.push_back(Edge(61, 46, 17.0));
    edges.push_back(Edge(46, 61, 17.0));
    edges.push_back(Edge(63, 49, 70.0));
    edges.push_back(Edge(61, 63, 60.0));
    edges.push_back(Edge(63, 61, 60.0));
    edges.push_back(Edge(49, 63, 70.0));
    edges.push_back(Edge(14, 21, 180.0));
    edges.push_back(Edge(21, 14, 180.0));
    edges.push_back(Edge(19, 49, 45.0));
    edges.push_back(Edge(49, 19, 45.0));
    edges.push_back(Edge(52, 20, 40.0));
    edges.push_back(Edge(20, 52, 40.0));
    edges.push_back(Edge(19, 52, 55.0));
    edges.push_back(Edge(52, 19, 55.0));
    edges.push_back(Edge(20, 49, 55.0));
    edges.push_back(Edge(49, 20, 55.0));
    edges.push_back(Edge(20, 24, 55.0));
    edges.push_back(Edge(24, 20, 55.0));
    edges.push_back(Edge(53, 24, 120.0));
    edges.push_back(Edge(24, 53, 120.0));
    edges.push_back(Edge(22, 23, 75.0));
    edges.push_back(Edge(23, 22, 75.0));
    edges.push_back(Edge(23, 26, 85.0));
    edges.push_back(Edge(26, 23, 85.0));
    edges.push_back(Edge(21, 59, 170.0));
    edges.push_back(Edge(59, 21, 170.0));
    edges.push_back(Edge(27, 59, 55.0));
    edges.push_back(Edge(59, 27, 55.0));
    edges.push_back(Edge(54, 59, 35.0));
    edges.push_back(Edge(59, 54, 35.0));
    edges.push_back(Edge(54, 28, 55.0));
    edges.push_back(Edge(28, 54, 55.0));
    edges.push_back(Edge(53, 59, 90.0));
    edges.push_back(Edge(59, 53, 90.0));
    edges.push_back(Edge(26, 53, 130.0));
    edges.push_back(Edge(53, 26, 130.0));
    edges.push_back(Edge(26, 55, 85.0));
    edges.push_back(Edge(55, 26, 85.0));
    edges.push_back(Edge(56, 57, 85.0));
    edges.push_back(Edge(57, 56, 85.0));
    edges.push_back(Edge(26, 57, 35.0));
    edges.push_back(Edge(57, 26, 35.0));
    edges.push_back(Edge(55, 56, 35.0));
    edges.push_back(Edge(56, 55, 35.0));
    edges.push_back(Edge(25, 58, 75.0));
    edges.push_back(Edge(58, 25, 75.0));
    edges.push_back(Edge(55, 58, 45.0));
    edges.push_back(Edge(58, 55, 45.0));
    edges.push_back(Edge(56, 29, 145.0));
    edges.push_back(Edge(29, 56, 145.0));
}

void MainWindow::drawMap() {
    //设置背景图
    QPixmap background(":/images/src/Home3.png");
    QGraphicsPixmapItem *backgroundItem = scene->addPixmap(background);
     backgroundItem->setZValue(-1); // 将背景图的 z 值设置为 -1，使它处于最底层



    // 先绘制边（连接节点的线）
    QPen edgePen(Qt::black, 1);
    for (const auto &edge : edges) {
        const Node &n1 = nodes[edge.getFrom()];
        const Node &n2 = nodes[edge.getTo()];
        scene->addLine(n1.getX(), n1.getY(), n2.getX(), n2.getY(), edgePen);
    }


    // 再绘制节点（小圆）
    /*QPen nodePen(Qt::black, 1);
    QBrush nodeBrush(Qt::blue);
    for (const auto &node : nodes) {

        scene->addEllipse(node.getX() - 5, node.getY() - 5, 10, 10, nodePen, nodeBrush);
        //QGraphicsTextItem *text = scene->addText(node.getName());
        //text->setPos(node.getX() + 5, node.getY() + 5);
    }
    */
}



void MainWindow::onPlanRouteClicked() {

    // 创建建筑物按钮
    createBuildingButtons();

}

void MainWindow::onNodeButtonClicked(int nodeId) {
    // 判断当前是设置起点还是终点
    if (startPoint == -1) {
        // 设置起点
        startPoint = nodeId;
        QMessageBox::information(this, "选择起点", "已选择起点: " + nodes[nodeId].getName());
    } else if (endPoint == -1) {
        // 设置终点
        endPoint = nodeId;
        QMessageBox::information(this, "选择终点", "已选择终点: " + nodes[nodeId].getName());

        // 询问用户是否选择途经点
        bool ok;
        QString text = QInputDialog::getText(this, "选择途经点", "请输入途经点的名称（多个点请用逗号分隔）", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            QStringList viaNames = text.split(",");
            for (const QString &name : viaNames) {
                // 根据途经点名称找到对应的节点 ID
                bool found = false;
                for (const Node &node : nodes) {
                    if (node.getName() == name.trimmed()) {
                        viaPoints.push_back(node.getId());
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    QMessageBox::warning(this, "无效途经点", "未找到途经点: " + name.trimmed());
                }
            }
        }

        // 计算路径
        planRoute(startPoint, endPoint);
    }
}

void MainWindow::planRoute(int startId, int endId) {
    // 计算路径
    std::vector<int> path = planner->calculateShortestPath(startId, endId);

    // 如果有途经点，则先计算起点到第一个途经点的路径
    if (!viaPoints.empty()) {
        path = planner->calculateShortestPath(startId, viaPoints[0]);
        for (size_t i = 0; i < viaPoints.size() - 1; ++i) {
            // 再计算途经点到下一个途经点的路径
            auto segmentPath = planner->calculateShortestPath(viaPoints[i], viaPoints[i+1]);
            path.insert(path.end(), segmentPath.begin() + 1, segmentPath.end());  // 去掉重复的点
        }
        // 最后计算途经点的最后一个到终点的路径
        auto finalPath = planner->calculateShortestPath(viaPoints.back(), endId);
        path.insert(path.end(), finalPath.begin() + 1, finalPath.end());  // 去掉重复的点
    } else {
        // 如果没有途经点，直接计算起点到终点的路径
        path = planner->calculateShortestPath(startId, endId);
    }

    // 如果有之前的路径，先删除它
    if (currentPathItemGroup) {
        scene->removeItem(currentPathItemGroup);  // 删除之前的路径
        delete currentPathItemGroup;  // 释放内存
        currentPathItemGroup = nullptr;  // 清空路径组
    }

    // 如果有之前的路径，先删除它
    if (currentPathItemGroup) {
        scene->removeItem(currentPathItemGroup);  // 删除之前的路径
        delete currentPathItemGroup;  // 释放内存
        currentPathItemGroup = nullptr;  // 清空路径组
    }

    // 如果路径计算成功，显示路径
    if (!path.empty()) {
        // 创建一个新的 QGraphicsItemGroup 来存储路径项
        currentPathItemGroup = new QGraphicsItemGroup();

        QPen pathPen(Qt::blue, 2);
        for (size_t i = 1; i < path.size(); ++i) {
            const Node &startNode = nodes[path[i-1]];
            const Node &endNode = nodes[path[i]];
            QGraphicsLineItem *lineItem = scene->addLine(startNode.getX(), startNode.getY(), endNode.getX(), endNode.getY(), pathPen);
            currentPathItemGroup->addToGroup(lineItem);  // 将新画的线条加入到路径组
        }

        scene->addItem(currentPathItemGroup);  // 将路径组添加到场景中
    } else {
        // 如果路径为空，显示错误消息
        QMessageBox::warning(this, "路径错误", "无法找到路径");
    }

    // 重置起点和终点
    startPoint = -1;
    endPoint = -1;
    viaPoints.clear();  // 清空途经点
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    // 获取鼠标点击的位置
    QPointF clickPos = graphicsView->mapToScene(event->pos());

    // 判断点击的区域
    if (clickPos.x() > 268 && clickPos.x() < 300 && clickPos.y() > 110 && clickPos.y() < 220) {
        // 点击区域，打开设备查询窗口
        DeviceSearchWindow *searchWindow = new DeviceSearchWindow(nodes, scene, this);
        searchWindow->exec();
    }

    if(clickPos.x() > 120 && clickPos.x() < 157 && clickPos.y() > 110 && clickPos.y() < 220){
        onPlanRouteClicked();
    }
}

void MainWindow::createBuildingButtons() {
    // 遍历所有节点，为建筑物节点创建按钮
    for (const auto &node : nodes) {
        if (node.getType() == BUILDING || node.getType() == TOLIET || node.getType() == RESTERANT) {
            // 为每个建筑物创建按钮
            QPushButton *nodeButton = new QPushButton(node.getName(), this);
            nodeButton->setGeometry(node.getX(), node.getY(), 50, 20);
            nodeButton->show();

            // 连接按钮的点击事件
            connect(nodeButton, &QPushButton::clicked, this, [this, node]() {
                onNodeButtonClicked(node.getId());  // 根据按钮点击选择起点或终点
            });
        }
    }
}


void MainWindow::openDeviceSearchWindow() {
    // 打开一个新窗口来选择设备类型，并传递 nodes 和 scene
    DeviceSearchWindow *searchWindow = new DeviceSearchWindow(nodes, scene, this);
    searchWindow->exec();
}



