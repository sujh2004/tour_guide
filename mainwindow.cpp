// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "routeinputdialog.h"
#include "route.h"
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(1000, 800);
    setWindowTitle("Tour——guide");

    // 创建 Map 类对象，负责显示地图
    map = new Map(this);
    map->setGeometry(0, 0, 1000, 800);  // 设置地图显示区域

    // 加载地图图像
    QPixmap mapImage(":/images/source/Home.png");
    map->setMapImage(mapImage);  // 设置地图

    // 创建 QLabel 来显示图片
    imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap(":/images/source/Home.png"));
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);
    imageLabel->resize(1024, 768);
    imageLabel->move(0, 0);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imageLabel);
    scrollArea->setGeometry(0, 0, 1000, 800);

    // 创建按钮
    button = new QPushButton("设置路线", this);
    button->setGeometry(50, 50, 100, 30);

    // 点击按钮弹出输入对话框
    connect(button, &QPushButton::clicked, this, &MainWindow::showRouteInputDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showRouteInputDialog()
{
    connect(button, &QPushButton::clicked, this, &MainWindow::showRouteInputDialog);
    connect(button,&QPushButton::clicked,this,&MainWindow::changeImage);
}


// 定义 drawRouteOnMap 函数
void MainWindow::drawRouteOnMap(const QString &startPoint, const QString &endPoint)
{
    // 使用 QPainter 在地图上绘制路径
    QPainter painter(imageLabel);
    painter.setPen(QPen(Qt::red, 3));  // 设置红色画笔和线宽

    // 假设我们有一个简化的方式来画直线路径
    QPoint start(100, 100);  // 假设起点坐标
    QPoint end(900, 600);    // 假设终点坐标
    painter.drawLine(start, end);  // 在地图上绘制直线
    imageLabel->repaint();  // 更新图像显示
}


//切换图片函数
void MainWindow::changeImage()
{
    // 切换图片
    imageLabel->setPixmap(QPixmap(":/images/source/map.jpg"));
}
