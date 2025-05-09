#include "migonggame.h"
#include <QPainter>
#include <QPushButton>
#include <QMessageBox>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>

// DraggableButton 类的实现
DraggableButton::DraggableButton(QWidget *parent)
    : QWidget(parent), isDragging(false), draggable(true)
{
    setFixedSize(50, 50);  // 设置按钮固定大小
}

DraggableButton::~DraggableButton()
{
}

void DraggableButton::setImage(const QPixmap &image)
{
    buttonImage = image;  // 设置按钮图像
    update();  // 更新界面
}

QPixmap DraggableButton::getButtonImage() const
{
    return buttonImage;  // 返回按钮图像
}

void DraggableButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    // 绘制按钮图片
    painter.drawPixmap(0, 0, width(), height(), buttonImage);
}

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
    if (draggable && event->button() == Qt::LeftButton) {
        isDragging = true;
        dragStartPos = event->pos();  // 记录按下鼠标时的位置
    }
}

void DraggableButton::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging && draggable) {
        QPoint offset = event->pos() - dragStartPos;  // 计算移动的偏移量
        move(pos() + offset);  // 移动按钮的位置
    }
}

void DraggableButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;  // 松开鼠标后停止拖动
    }
}

// GameMigong 类的实现
GameMigong::GameMigong(QWidget *parent)
    : QWidget(parent),
    isGameOver(false), isGameStarted(false), moveStep(5), currentLevel(1)
{
    setFixedSize(650, 600);  // 设置窗口大小为800x600

    player = QRect(50, height() / 2 - 25, 50, 50);  // 小人初始位置
    playerDirection = QPoint(1, 0);  // 默认向右

    target = QRect(500, height() / 2 - 25, 50, 50);  // 目标位置

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameMigong::movePlayer);
    timer->start(100);  // 每100毫秒更新一次小人位置
buttonposition();
    loadLevel(currentLevel);  // 加载第一关

    // 添加开始游戏按钮
    QPushButton *startButton = new QPushButton("开始游戏", this);
    startButton->setGeometry(250, 50, 100, 40);
    connect(startButton, &QPushButton::clicked, this, &GameMigong::startGame);

    QPushButton *restartButton = new QPushButton("重新开始", this);
    restartButton->setGeometry(350, 50, 100, 40);
    connect(restartButton, &QPushButton::clicked, this, &GameMigong::restart);
}

GameMigong::~GameMigong()
{
}

void GameMigong::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 绘制背景图
    QPixmap background(":/images/src/migongbackground.jpg");  // 加载背景图
    painter.drawPixmap(0, 0, width(), height(), background);  // 绘制背景图

    // 绘制小人图像
    painter.drawPixmap(player, playerPixmap);

    // 绘制目标图像
    painter.drawPixmap(target, targetPixmap);

    // 绘制障碍物图像
    for (const QRect &obstacle : obstacles) {
        painter.drawPixmap(obstacle, obstaclePixmap);
    }

    // 绘制按钮图像
    for (DraggableButton *button : buttons) {
        painter.drawPixmap(button->geometry(), button->buttonImage);  // 使用 getter 方法
    }
}

void GameMigong::movePlayer()
{
    if (isGameStarted && !isGameOver) {
        player.moveTopLeft(player.topLeft() + playerDirection * moveStep);  // 根据方向移动小人

        checkButtonCollisions();  // 检查是否与按钮发生碰撞
        checkCollisions();  // 检查是否与障碍物碰撞
       /* if(isGameStarted){
            for (DraggableButton *button : buttons) {
                qDebug() << "Button position: " << button->geometry().topLeft();
                QPixmap buttonImage = button->getButtonImage();
               qDebug() <<"buttonnumb  "<< buttonImage.cacheKey();

           }
            qDebug() << "Player position: " << player.topLeft();
        }*/
        // 如果到达终点，显示成功提示并进入下一关
        if (player.intersects(target)) {
            isGameOver = true;
            showSuccessMessage();
            if (currentLevel == 1) {
                isGameOver = false;
                for (DraggableButton *button : buttons) {
                    button->draggable=true; // 不再需要设置按钮的拖动功能
                }

                currentLevel = 2;  // 切换到第二关
                startNewLevel(currentLevel);
                player = QRect(50, height() / 2 - 25, 50, 50);  // 小人初始位置
                playerDirection = QPoint(1, 0);  // 默认向右
            }
        }

        update();  // 更新界面
    }
}

void GameMigong::checkCollisions()
{
    // 检查小人与障碍物的碰撞
    for (const QRect &obstacle : obstacles) {
        if (player.intersects(obstacle)) {
            isGameOver = true;  // 碰撞后结束游戏
            break;
        }
    }
}

void GameMigong::checkButtonCollisions()
{

        // 遍历所有按钮并检查与玩家的碰撞
        for (DraggableButton *button : buttons) {
            // 检查按钮是否与玩家碰撞
            if (player.intersects(button->geometry())) {
                // 如果按钮的区域包含玩家的中心点，表示玩家正在点击按钮
                if (button->geometry().contains(player.center())) {
                    if (button->buttonImage.cacheKey()== QPixmap(":/images/src/xiangshang.png").cacheKey()) {
                        playerDirection = QPoint(0, -1);  // 向上移动
                        qDebug() << button->buttonImage.cacheKey();
                    } else if (button->buttonImage.cacheKey() == QPixmap(":/images/src/xia.png").cacheKey()) {
                        playerDirection = QPoint(0, 1);  // 向下移动
                    } else if (button->buttonImage.cacheKey() == QPixmap(":/images/src/zuo.png").cacheKey()) {
                        playerDirection = QPoint(-1, 0);  // 向左移动
                    } else if (button->buttonImage.cacheKey() == QPixmap(":/images/src/xiangyou.png").cacheKey()) {
                        playerDirection = QPoint(1, 0);  // 向右移动
                    }
                }
            }
        }
    }


void GameMigong::loadLevel(int level)
{
    // 清空上一关的按钮
   // buttons.clear();  // 清空按钮列表

    // 根据关卡加载不同的障碍物、按钮和图像
    if (level == 1) {
        obstacles.append(QRect(200, height() / 2 - 50, 50, 50));  // 第一关的障碍物
        spawnButtons(level);  // 生成按钮
        player = QRect(50, height() / 2 - 25, 50, 50);  // 重置小人初始位置
        playerPixmap = QPixmap(":/images/src/migongcharacter.png");  // 加载第一关的小人图像
        obstaclePixmap = QPixmap(":/images/src/xianjing.png");  // 加载障碍物图像
        targetPixmap = QPixmap(":/images/src/zhongdian.png");  // 加载终点图像
    } else if (level == 2) {
        obstacles.append(QRect(500, height() / 2 - 75, 50, 50));
        obstacles.append(QRect(400, height() / 2 - 25, 50, 50));
        obstacles.append(QRect(500, height() / 2 + 75, 50, 50));
        obstacles.append(QRect(450, height() / 2 - 75, 50, 50));
        spawnButtons(level);  // 生成按钮
        player = QRect(50, height() / 2 - 25, 50, 50);  // 重置小人初始位置
        playerPixmap = QPixmap(":/images/src/migongcharacter.png");  // 加载第二关的小人图像
        obstaclePixmap = QPixmap(":/images/src/xianjing.png");  // 加载障碍物图像
        targetPixmap = QPixmap(":/images/src/zhongdian.png");  // 加载终点图像
    }
}

void GameMigong::spawnButtons(int level)
{
    // 清空按钮列表
   // buttons.clear();

    QPixmap upPixmap(":/images/src/xiangshang.png");  // 向上按钮图片
    QPixmap rightPixmap(":/images/src/xiangyou.png");  // 向右按钮图片
    QPixmap downPixmap(":/images/src/xia.png");  // 向下按钮图片
    QPixmap leftPixmap(":/images/src/zuo.png");  // 向左按钮图片

    if(level==1){
        // 第一关设置方向键


        DraggableButton *upButton = new DraggableButton(this);
        upButton->setGeometry(100, 100, 50, 50);
        upButton->setImage(upPixmap);
        buttons.append(upButton);
       // QPixmap buttonImage = upButton->getButtonImage();
         //qDebug() << buttonImage.cacheKey();

         DraggableButton *rightButton = new DraggableButton(this);
        rightButton->setGeometry(200, 100, 50, 50);
        rightButton->setImage(rightPixmap);
        buttons.append(rightButton);


        DraggableButton *downButton = new DraggableButton(this);
        downButton->setGeometry(300, 100, 50, 50);
        downButton->setImage(downPixmap);
        buttons.append(downButton);
        //QPixmap buttonImage2 = downButton->getButtonImage();
        //qDebug() << buttonImage2.cacheKey();



    }
    else if(level==2){
        // 第二关设置方向键（6个按钮）

        DraggableButton *rightButton2 = new DraggableButton(this);
        rightButton2->setGeometry(400, 100, 50, 50);
        rightButton2->setImage(rightPixmap);
        buttons.append(rightButton2);

        DraggableButton *leftButton1 = new DraggableButton(this);
        leftButton1->setGeometry(500, 100, 50, 50);
        leftButton1->setImage(leftPixmap);
        buttons.append(leftButton1);

        DraggableButton *leftButton2 = new DraggableButton(this);
        leftButton2->setGeometry(600, 100, 50, 50);
        leftButton2->setImage(leftPixmap);
        buttons.append(leftButton2);

    }
}


void GameMigong::startGame()
{
    isGameStarted = true;  // 开始游戏
    for (DraggableButton *button : buttons) {
        button->draggable=false; // 不再需要设置按钮的拖动功能
    }
    isGameOver = false;
}

void GameMigong::restart()
{
    isGameStarted = false;  // 结束游戏
    isGameOver = false;
    currentLevel = 1;  // 重置关卡

    // 重新生成按钮
    //spawnButtons(currentLevel);  // 重新生成按钮
    for (DraggableButton *button : buttons) {
        button->draggable=true; // 不再需要设置按钮的拖动功能
    }
    // 重置小人位置和方向
    player = QRect(50, height() / 2 - 25, 50, 50);  // 小人初始位置
    playerDirection = QPoint(1, 0);  // 默认向右

    update();  // 更新界面
}

void GameMigong::showSuccessMessage()
{
    // 游戏胜利后显示成功提示框
    QMessageBox::information(this, "游戏成功", "恭喜你，成功到达终点！");
    for (DraggableButton *button : buttons) {
        button->draggable=false; // 不再需要设置按钮的拖动功能
    }
    isGameStarted = false;  // 停止游戏

}

void GameMigong::startNewLevel(int level)
{
    // 删除当前窗口
    this->close();  // 关闭当前的窗口

    // 创建新的窗口并显示
    GameMigong *newGame = new GameMigong();  // 创建新的游戏对象
    newGame->setLevel(level);  // 设置新关卡
    newGame->show();  // 显示新窗口
}

void GameMigong::setLevel(int level)
{
    currentLevel = level;  // 设置当前关卡
    loadLevel(currentLevel);  // 加载该关卡
    update();  // 更新界面
}
void GameMigong::buttonposition(){
    if(isGameStarted){
    for (DraggableButton *button : buttons) {
    qDebug() << "Button position: " << button->geometry().topLeft();

    }
     qDebug() << "Player position: " << player.topLeft();
    }
}
