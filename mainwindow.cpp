#include "mainwindow.h"
#include "favoritewidget.h"
#include "musicplayer.h"
#include <QVBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup> // 用于存储路径的图形项
#include <QMouseEvent>
#include <QMessageBox>
#include <QThread>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDialog>  // 使用 QDialog 作为新的窗口类型
#include <QSettings>>
#include <QFont>
#include <QFontDatabase>

//int maptype=0;//0表示未进入寻路模式
//test

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    mainPage(new QWidget(this)),  // 创建主页面
    //centralWidget(new QWidget(this)),  // 初始化 centralWidget
    graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    planRouteButton(new QPushButton("规划路线", this)),
    stackedWidget(new QStackedWidget(this)),  // 初始化页面管理器
    puzzlePage(new PuzzleWidget(this)),  // 创建拼图游戏页面
    planner(nullptr),
    currentPathItemGroup(nullptr),
    musicplayer(new MusicPlayer(this))
{
    resize(650,950);  // 设置初始窗口大小为 650×950
    setCentralWidget(stackedWidget);
    playerController = new PlayerController(scene, nodes, this);

    // 创建主页面的布局
    //mainPage->setLayout(new QVBoxLayout());  // 创建布局
    //mainPage->layout()->addWidget(gameButton);  // 将游戏按钮添加到主页面的布局

    // 将页面添加到 QStackedWidget
    stackedWidget->addWidget(mainPage);  // 主页面
    stackedWidget->addWidget(puzzlePage);  // 拼图游戏页面

    // 默认显示主页面
    stackedWidget->setCurrentWidget(mainPage);
    //介绍页面
    introWidget = new IntroWidget(this);
    introWidget->setFixedSize(325, 250);
    // 设置位置：画面居中
    int x = (this->width() - introWidget->width()) / 2;
    int y = (this->height() - introWidget->height()) / 2;
    introWidget->move(x, y);
    introWidget->hide();
    musicplayer->playMusic("qrc:/Music/src/bgm.mp3");
    musicplayer->setLooping(true);
    setupUI();  // 设置界面
    initData();  // 初始化数据
    drawMap();  // 绘制地图
    this->setFocusPolicy(Qt::StrongFocus);
    // 创建路径规划器
    planner = new RoutePlanner(nodes, edges);
}

MainWindow::~MainWindow() {
    //删除收藏夹记录
    QSettings settings("MyApp", "TourGuide");
    settings.beginGroup("Favorites");
    settings.remove("");
    settings.endGroup();
    delete planner;
}

void MainWindow::creategamebotton() {
    // 创建游戏按钮
    gameButton = new QPushButton("进入拼图游戏", this);  // 创建按钮

    // 设置游戏按钮的大小
    gameButton->resize(100, 50);  // 设置按钮的大小为 200x50

    // 设置游戏按钮的位置
    gameButton->move(550, 0);  // 设置按钮的位置为 (225, 400)
    connect(gameButton, &QPushButton::clicked, this, &MainWindow::onGameButtonClicked);  // 连接点击事件

}

void MainWindow::setupUI() {
    // 设置中央部件的布局
    QVBoxLayout *layout = new QVBoxLayout;
    mainPage->setLayout(layout);

    //设置统一字体
    int id = QFontDatabase::addApplicationFont(":/fonts/HanyiSentyScholar.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(fontFamily);
    qApp->setFont(font);

    // 设置 QGraphicsView
    graphicsView->setScene(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(graphicsView);
    planRouteButton->move(100, 150);  // 设置按钮位置为 (x=100, y=150)
    planRouteButton->setStyleSheet("background: transparent; border: none; color: transparent;");
    // 添加按钮
    //layout->addWidget(planRouteButton);

    //------收藏夹入口 BEGIN-----
    // 创建按钮并设置位置
    showFavoriteButton = new QPushButton("Show Favorite", this);
    showFavoriteButton->setGeometry(435, 95, 40, 140);  // 设置位置 (500, 150) 和尺寸 (100, 30)
    showFavoriteButton->setCursor(Qt::PointingHandCursor);
    showFavoriteButton->setStyleSheet("QPushButton {"
                                        "   background-color: transparent;"  // 设置按钮的背景为透明
                                        "   border: none;"                   // 去掉边框
                                        "}"
                                        "QPushButton:hover {"
                                        "   background-color: transparent;"  // 悬浮时背景保持透明，不变
                                        "}");
    showFavoriteButton->setAttribute(Qt::WA_TransparentForMouseEvents, false);  // 确保按钮可以接收鼠标事件
    showFavoriteButton->setText("");
    // 连接按钮点击事件
    connect(showFavoriteButton, &QPushButton::clicked, this, &MainWindow::on_showFavoriteButton_clicked);
     //------收藏夹入口 END-----
}

void MainWindow::initData() {
    // 示例：创建一些节点
    nodes.push_back(Node(0, 75, 235, "西北角楼", BUILDING, 0, false,
                         "西北角楼是故宫四座角楼之一，以其精美的建筑风格和复杂的斗拱结构而著称。"
                         "角楼拥有九梁十八柱七十二条脊，是中国古代建筑艺术的代表作，"
                         "常年吸引大量游客驻足观赏。",
                         ":/buildings/buildings/0.jpg"));
    nodes.push_back(Node(1, 285, 235, "神武门", BUILDING, 0, false,
                         "神武门位于故宫北侧，是紫禁城的北大门。历史上曾为皇帝出入的重要通道，"
                         "如今是故宫博物院的主要出口之一，其建筑风格庄严大气，体现出皇家威仪。",
                         ":/buildings/buildings/1.jpg"));
    nodes.push_back(Node(2, 504, 235, "东北角楼",BUILDING, 0, false,
                         "位于故宫的东北角，是防御性建筑之一，结构独特，位置险要，常作为游客欣赏故宫全景的好地方。"));
    nodes.push_back(Node(3, 285, 310, "御花园", BUILDING, 0, false,
                         "御花园是故宫的皇家园林，位于紫禁城的北端。园中亭台楼阁错落有致，植被茂盛，"
                         "布局精巧，是皇室成员休憩与赏景的佳地，也展示了古代园林艺术的极致。",
                         ":/buildings/buildings/3.jpg"));
    nodes.push_back(Node(4, 505, 310, "乐寿堂",BUILDING,0,false,"为清朝皇帝与皇太后的休养之地，典雅宁静，宫殿装饰精美，展示了清代皇室的奢华与优雅。"));
    nodes.push_back(Node(5, 285, 370, "坤宁宫",BUILDING,0,false,"清朝时期的皇后寝宫，是故宫中保存较为完好的宫殿之一，气氛庄重，体现了皇后的威仪与宫廷生活。"));
    nodes.push_back(Node(6, 285, 445, "乾清宫", BUILDING, 0, false,
                         "乾清宫为内廷正宫，是清代皇帝居住和处理政务的重要场所。宫内陈设华丽，雕梁画栋，"
                         "彰显皇权的威严与尊贵，是故宫建筑群中最具代表性的宫殿之一。",
                         ":/buildings/buildings/6.jpg"));
    nodes.push_back(Node(7, 375, 445, "景仁宫",BUILDING,0,false,"景仁宫位于故宫内，是皇帝的后宫之一，历史悠久。宫殿精致典雅，曾是皇后和妃子们的居所，充满皇家气息。"));
    nodes.push_back(Node(8, 505, 425, "宁寿宫",BUILDING,0,false,"位于故宫西侧，是慈禧太后的居所，宫内建筑华丽，历史底蕴深厚，展现清朝皇家生活的奢华与庄严。"));
    nodes.push_back(Node(9, 110, 390, "寿安宫",BUILDING,0,false,"曾为清朝皇太后的寝宫，宫内布局精美，气氛宁静，展现了清朝后宫的威严与典雅。"));
    nodes.push_back(Node(10, 320, 470, "斋宫",BUILDING,0,false,"位于故宫内，是皇帝日常处理政务的地方之一，古朴庄重，具有浓厚的历史气息和皇室气派。"));
    nodes.push_back(Node(11, 235, 470, "养心殿",BUILDING,0,false,"清朝皇帝的日常办公场所，宫殿内装饰精美，是清代重要的政治决策中心之一。"));
    nodes.push_back(Node(12, 80, 480, "寿康宫",BUILDING,0,false,"原为皇太后的寝宫，布局严谨且典雅，是清代宫殿建筑中的代表之一，展现了皇室的奢华生活。"));
    nodes.push_back(Node(13, 237, 400, "翊坤宫",BUILDING,0,false,"曾是皇太后的居住地，环境清幽，古典宫殿建筑风格突显皇家气派，是清宫文化的重要象征。"));
    nodes.push_back(Node(14, 505, 480, "皇极殿",BUILDING,0,false,"古老的宫殿，曾为皇帝举行重大仪式的场所，建筑庄重且气势恢宏，是故宫的重要部分。"));
    nodes.push_back(Node(15, 153, 420, "雨花阁",BUILDING,0,false,"位于故宫东南侧，以其独特的建筑风格和历史背景著称，是古代皇室文人雅集之地。"));
    nodes.push_back(Node(16, 155, 565, "慈宁宫",BUILDING,0,false,"清朝时期，慈禧太后的居所之一，环境安静且庄严，展现了浓厚的皇室风格。"));
    nodes.push_back(Node(17, 285, 573, "保和殿",BUILDING,0,false,"皇帝举行重要典礼和大典的场所，殿内宽敞庄严，常举行朝会、封禅大典等。"));
    nodes.push_back(Node(18, 285, 611, "中和殿",BUILDING,0,false,"故宫的重要宫殿，历代皇帝即位大典的场所之一，气势雄伟，极具历史价值。"));
    nodes.push_back(Node(19, 370, 600, "剑亭",BUILDING,0,false,"位于故宫内，是皇帝指挥军事或举行仪式的地方，气氛肃穆，充满历史感"));
    nodes.push_back(Node(20, 415, 655, "文渊阁",BUILDING,0,false,"典藏着大量的皇家图书和文物，是故宫内重要的文化和学术交流场所。"));
    nodes.push_back(Node(21, 505, 660, "南三所",BUILDING,0,false,"清朝皇帝的寝宫所在，古色古香，典型的宫殿建筑，展示了清代宫廷的日常生活。"));
    nodes.push_back(Node(22, 285, 670, "太和殿",BUILDING,0,false,"是故宫最重要的殿宇之一，盛大雄伟，历史上皇帝即位、朝会等重大政治活动均在此举行。"));
    nodes.push_back(Node(23, 285, 745, "太和门",BUILDING,0,false,"故宫的正门，气势宏伟，是游客进入故宫的主要入口，历史上也是皇帝接受朝拜的地方。"));
    nodes.push_back(Node(24, 415, 710, "文华殿",BUILDING,0,false,"曾是清朝皇帝办公和接见大臣的地方，建筑雄伟典雅，展示了皇室的文化底蕴。"));
    nodes.push_back(Node(25, 155, 755, "武英殿",BUILDING,0,false,"皇帝的办公场所之一，主要用于接见大臣和举行政务，殿内装饰富丽堂皇。"));
    nodes.push_back(Node(26, 285, 830, "午门",BUILDING,0,false,"故宫正门，是宫殿的主入口，结构宏伟，是古代皇宫的重要标志。"));
    nodes.push_back(Node(27, 560, 830, "东华门",BUILDING,0,false,"位于故宫东侧，是宫殿的另一个重要入口，具有深厚的历史文化价值。"));
    nodes.push_back(Node(28, 560, 865, "东南角楼",BUILDING,0,false,"落于故宫的东南角，作为防御工事之一，角楼建筑独特，景色迷人。"));
    nodes.push_back(Node(29, 55, 865, "西南角楼",BUILDING,0,false,"位于故宫西南角，与东南角楼互为呼应，建筑独具特色，常被作为故宫的标志性景观之一。"));
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
    // 如果场景中已经有背景图，先删除它
    QList<QGraphicsItem *> items = scene->items();
    for (QGraphicsItem *item : items) {
        if (dynamic_cast<QGraphicsPixmapItem *>(item)) {
            scene->removeItem(item);
            delete item;  // 删除原背景图
        }
    }
    QGraphicsPixmapItem *backgroundItem = scene->addPixmap(background);
     backgroundItem->setZValue(-1); // 将背景图的 z 值设置为 -1，使它处于最底层



    // 先绘制边（连接节点的线）
    QPen edgePen(Qt::black, 1);
    for (const auto &edge : edges) {
        const Node &n1 = nodes[edge.getFrom()];
        const Node &n2 = nodes[edge.getTo()];
        scene->addLine(n1.getX(), n1.getY(), n2.getX(), n2.getY(), edgePen);
    }

    onPlanRouteClicked();
     creategamebotton();
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

void MainWindow::planRoute(int startId, int endId) {
    // 清理旧路径相关内容
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    // 如果有之前的路径，先删除它
    if (currentPathItemGroup) {
        scene->removeItem(currentPathItemGroup);  // 删除之前的路径
        delete currentPathItemGroup;  // 释放内存
        currentPathItemGroup = nullptr;  // 清空路径组
    }

    // 清除旧路径段
    for (QGraphicsLineItem* item : lineItems) {
        scene->removeItem(item);  // 从场景中移除
        delete item;              // 释放内存
    }
    lineItems.clear();  // 清空容器
    currentIndex = 0;   // 重置索引

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



    // 如果路径计算成功，显示路径
    if (!path.empty()) {
        // 创建一个新的 QGraphicsItemGroup 来存储路径项
        currentPathItemGroup = new QGraphicsItemGroup();

        QPen pathPen(Qt::blue, 2);

        // 预先存储所有的路径段
        for (size_t i = 1; i < path.size(); ++i) {
            const Node &startNode = nodes[path[i-1]];
            const Node &endNode = nodes[path[i]];

            QGraphicsLineItem *lineItem = new QGraphicsLineItem(startNode.getX(), startNode.getY(),
                                                                endNode.getX(), endNode.getY());
            lineItem->setPen(pathPen);
            lineItems.push_back(lineItem);  // 保存每一段路径
        }

        // 启动定时器每隔1秒显示一段路径
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::showNextLine);
        timer->start(1000);  // 每1秒显示一段路径

        // 将路径组添加到场景中，但先不显示路径
        scene->addItem(currentPathItemGroup);
    } else {
        // 如果路径为空，显示错误消息
        QMessageBox::warning(this, "路径错误", "无法找到路径");
    }

    // 重置起点和终点
    startPoint = -1;
    endPoint = -1;
    viaPoints.clear();  // 清空途经点
    playerController->setPath(path);
    playerController->setPathLines(&lineItems);  // 提供路径线引用
}

// 定时器槽函数：逐步显示路径
void MainWindow::showNextLine() {
    if (currentIndex < lineItems.size()) {
        // 将当前路径段添加到场景中
        scene->addItem(lineItems[currentIndex]);
        ++currentIndex;  // 更新当前索引
    } else {
        // 所有路径段显示完成，停止定时器
        timer->stop();
        delete timer;  // 删除定时器
        timer = nullptr;  // 清空定时器
    }
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
    if (clickPos.x() > 120 && clickPos.x() < 157 && clickPos.y() > 110 && clickPos.y() < 220 ){
        switch (maptype) {
          case 0:  // 进入寻路模式
            maptype = 1;
            onPlanRouteClicked();
             QMessageBox::information(this, "寻路模式","您已进入寻路模式");
            break;
          case 1:
            // 退出寻路模式
            maptype = 0;
            resetRoute();
            break;

        }

    }
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

void MainWindow::setupButtonUI(QPushButton* button) {
    // 加载自定义字体
    int fontId = QFontDatabase::addApplicationFont(":/fonts/HanyiSentyScholar.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);  // 获取字体家族名称

    // 设置按钮字体
    QFont font(fontFamily, 14, QFont::Bold);
    button->setFont(font);

    // 设置按钮背景颜色和文字颜色
    button->setStyleSheet("QPushButton {"
                          "background-color: #C8B5B3;"   // 背景颜色
                          "color: #594C57;"                 // 文字为白色
                          "font-size:12px;"                //文字大小
                          "border-radius: 5px;"           // 圆角边框
                          "}");

    // 设置按钮在悬浮时的样式
    button->setStyleSheet(button->styleSheet() +
                          "QPushButton:hover {"
                          "background-color: #928187;"  // 悬浮时背景颜色变暗
                          "}");

    // 设置按钮在按下时的样式
    button->setStyleSheet(button->styleSheet() +
                          "QPushButton:pressed {"
                          "background-color: #8F3D2C;"  // 按下时背景颜色变更
                          "color:white;"                //文字颜色
                          "}");

    // 设置按钮边框的圆角
    button->setStyleSheet(button->styleSheet() +
                          "QPushButton {"
                          "border-radius: 10px;"  // 设置圆角大小
                          "}");

    // 设置按钮的大小策略
    button->setFixedSize(50, 20);  // 设置固定大小，或根据实际需求调整
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (QPushButton *button = qobject_cast<QPushButton *>(watched)) {
        if (event->type() == QEvent::Enter) {
            qDebug()<<button->text();
            for (const auto &node : nodes) {
                if (node.getName() == button->text()) {//找到按钮对应的景点
                    introWidget->showIntro(node);
                    break;
                }
            }
        } else if (event->type() == QEvent::Leave) {
            introWidget->hideIntro();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::createBuildingButtons() {
    // 删除旧按钮
    for (QPushButton* btn : buildingButtons) {
        btn->hide();
        delete btn;
    }
    buildingButtons.clear();

    // 遍历所有节点，为建筑物节点创建按钮
    for (const auto &node : nodes) {
        if (node.getType() == BUILDING || node.getType() == TOLIET || node.getType() == RESTERANT) {
            QPushButton *nodeButton = new QPushButton(node.getName(), this);
            setupButtonUI(nodeButton);
            nodeButton->setGeometry(node.getX(), node.getY(), 50, 20);
            nodeButton->show();

            buildingButtons.push_back(nodeButton);  // 记录按钮

            // 设置连接
            if (maptype == 1) {
                connect(nodeButton, &QPushButton::clicked, this, [this, node]() {
                    onNodeButtonClicked(node.getId());
                });
            } else {
                connect(nodeButton, &QPushButton::clicked, this, [this, node]() {
                    showBuildingDetails(node.getId());
                });
            }
             nodeButton->installEventFilter(this); // For 页面介绍 安装过滤器监测hover
            introWidget->raise();  //让它浮在最上层
        }
    }
}

void MainWindow::openDeviceSearchWindow() {
    // 打开一个新窗口来选择设备类型，并传递 nodes 和 scene
    DeviceSearchWindow *searchWindow = new DeviceSearchWindow(nodes, scene, this);
    searchWindow->exec();
}

void MainWindow::showBuildingDetails(int nodeId) {
    // 获取建筑物节点
    const Node &node = nodes[nodeId];

    // 根据节点类型构造建筑物详细信息
    QString typeStr;
    switch (node.getType()) {
    case BUILDING:
        typeStr = "建筑物";
        break;
    case INTERSECTION:
        typeStr = "路口";
        break;
    case TOLIET:
        typeStr = "卫生间";
        break;
    case RESTERANT:
        typeStr = "饭店";
        break;
    default:
        typeStr = "未知类型";
        break;
    }

    QString details = QString("名称: %1\n类型: %2\n位置: (%3, %4)")
                          .arg(node.getName())
                          .arg(typeStr)
                          .arg(node.getX())
                          .arg(node.getY());

    // 弹出消息框显示建筑物的详细信息
    QMessageBox::information(this, "建筑物详细信息", details);
}

void MainWindow::resetRoute() {
    // 如果有之前的路径，先删除它
    if (currentPathItemGroup) {
        scene->removeItem(currentPathItemGroup);  // 删除之前的路径
        delete currentPathItemGroup;  // 释放内存
        currentPathItemGroup = nullptr;  // 清空路径组

    }

    // 重置起点和终点
    startPoint = -1;
    endPoint = -1;

    // 重新绘制地图，恢复初始状态
    drawMap();  // 重新绘制地图背景和建筑物节点等
     maptype = 0;
    // 可选：弹出提示，告知用户路径已被重置
    QMessageBox::information(this, "路径重置", "路径规划已被取消，画面已重置。");
    playerController->reset();
}

void MainWindow::onGameButtonClicked() {
    // 创建新的拼图游戏窗口
    PuzzleWidget *puzzleWindow = new PuzzleWidget();  // 创建新的拼图窗口
    puzzleWindow->setWindowTitle("拼图游戏");  // 设置窗口标题
    puzzleWindow->resize(800, 1200);  // 设置窗口大小
    puzzleWindow->show();  // 显示窗口
}

NodeList* MainWindow::getNodes() {
    NodeList* list = new NodeList(nodes);  // 将 std::vector<Node> 转换为 NodeList
    return list;
}

void MainWindow::on_showFavoriteButton_clicked()
{
    // 创建 FavoriteWidget，并将 MainWindow 作为父对象
    FavoriteWidget *favoriteWidget = new FavoriteWidget(this);  // 'this' 代表 MainWindow
    favoriteWidget->loadFavoriteList();
    favoriteWidget->show();  // 显示 FavoriteWidget
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (!playerController) return;

    switch (event->key()) {
    case Qt::Key_Right:
    case Qt::Key_D:
    case Qt::Key_Down:
    case Qt::Key_S:
        playerController->moveForward();
        break;
    case Qt::Key_Left:
    case Qt::Key_A:
    case Qt::Key_Up:
    case Qt::Key_W:
        playerController->moveBackward();
        break;
    }
}
