#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //固定窗口大小
    setFixedSize(1000,800);

    //设置窗口标题
    setWindowTitle("Tour——guide");


    //用于创建Qlabel用于显示图片
    imageLabel = new QLabel(this);
    imageLabel->setPixmap(QPixmap(":/images/source/Home.png"));
    imageLabel->resize(1024,768);
    imageLabel->move(0,0); // 设定 QLabel 位置

    //创建按钮
    button = new QPushButton("苏京辉",this);
    button ->setGeometry(50,50,50,20);

    //点击按钮切换到地图界面
    connect(button,&QPushButton::clicked,this,&MainWindow::changeImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//切换图片函数
void MainWindow::changeImage()
{
    // 切换图片
    imageLabel->setPixmap(QPixmap(":/images/source/map.jpg"));  // 这里换成你想显示的新图片路径
}
