#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStackedWidget>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QGraphicsLineItem>
#include <QTimer>
#include <QInputDialog>
#include <QGraphicsItemGroup>
#include <vector>
#include "node.h"
#include "edge.h"
#include "nodelist.h"
#include "routeplanner.h"
#include "devicesearchwindow.h"
#include "puzzlewidget.h"  // 引入拼图游戏界面
#include "favoritewidget.h"
#include "playercontroller.h" //添加人物
#include "introwidget.h"
#include "musicplayer.h"
#include "migonggame.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 提供公共方法以获取预设的节点集合
    NodeList* getNodes();

private slots:
    void onPlanRouteClicked();
    void openDeviceSearchWindow();  // 添加此函数声明
    void onGameButtonClicked();  // 点击拼图游戏按钮的槽函数
    void onmigongbuttonclicked();//点击迷宫游戏

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;//For 悬浮景点按钮页面介绍

private:
    //QWidget *centralWidget;   // 中央部件
    QWidget *mainPage;  // 主页面
    QGraphicsView *graphicsView; // 显示地图的视图
    QGraphicsScene *scene;    // 场景，管理所有图形项
    QPushButton *planRouteButton; // 规划路线的按钮
    QPushButton *gameButton;  // 进入拼图游戏的按钮
    QPushButton *showFavoriteButton; //进入收藏夹的按钮
    PlayerController *playerController;//添加人物
    QStackedWidget *stackedWidget;  // 页面管理器
    PuzzleWidget *puzzlePage;  // 拼图游戏页面
    GameMigong *migongPgae;//迷宫游戏页面


    IntroWidget* introWidget;//景点介绍页面
    QList<QPushButton*> buildingButtons;  // 管理所有建筑按钮
    std::vector<Node> nodes;  // 存储节点
    std::vector<Edge> edges;  // 存储边
    RoutePlanner *planner;    // 路径规划器
    std::vector<int> pathToDisplay;  // 存储路径
    QVector<QGraphicsLineItem*> lineItems; // 存储每一段路径
    qsizetype currentIndex = 0;  // 当前显示到路径的哪一段
    QTimer* timer = nullptr;  // 定时器
    int startPoint = -1;  // 起点
    int endPoint = -1;    // 终点

    int maptype=0;

    std::vector<int> viaPoints;

    QGraphicsItemGroup *currentPathItemGroup;  // 存储当前高亮路径的图形项组
    MusicPlayer *musicplayer;
    QAudioOutput *audioOutput;

    void setupUI();  // 设置界面
    void initData(); // 初始化数据（节点、边）
    void drawMap();  // 绘制地图
    void showNextLine();  // 用于定时显示路径段
    void onNodeButtonClicked(int nodeId);
    void planRoute(int startId, int endId);
    void createBuildingButtons();
    void setupButtonUI(QPushButton* button);
    void showBuildingDetails(int nodeId);
    void resetRoute();
    void creategamebotton();
    void createmigonggamebotton();
    void on_showFavoriteButton_clicked();//进入收藏夹的函数
};
#endif // MAINWINDOW_H
