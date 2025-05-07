#include "migonggame.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>

DraggableButton::DraggableButton(QWidget *parent)
    : QWidget(parent), isDragging(false), draggable(true)
{
    setFixedSize(50, 50);  // 设置按钮固定大小
}

DraggableButton::~DraggableButton()
{
}

void DraggableButton::setText(const QString &text)
{
    buttonText = text;
    update();
}

void DraggableButton::setDraggable(bool draggable)
{
    this->draggable = draggable;
}

void DraggableButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    painter.setBrush(Qt::yellow);
    painter.drawRect(0, 0, width(), height());
    painter.setPen(Qt::black);
    painter.drawText(rect(), Qt::AlignCenter, buttonText);
}

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
    if (draggable && event->button() == Qt::LeftButton) {
        isDragging = true;
        dragStartPos = event->pos();
    }
}

void DraggableButton::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging && draggable) {
        QPoint offset = event->pos() - dragStartPos;
        move(pos() + offset);
    }
}

void DraggableButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}

GameMigong::GameMigong(QWidget *parent)
    : QWidget(parent), isGameOver(false), isGameStarted(false), moveStep(5), currentLevel(1)
{
    player = QRect(50, height() / 2 - 25, 50, 50);  // 初始位置
    playerDirection = QPoint(1, 0);  // 默认向右

    target = QRect(width() - 100, height() / 2 - 25, 50, 50);  // 目标位置

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameMigong::movePlayer);
    timer->start(100);

    loadLevel(currentLevel);  // 加载第一关

    QPushButton *startButton = new QPushButton("Start Game", this);
    startButton->setGeometry(250, 50, 100, 40);
    connect(startButton, &QPushButton::clicked, this, &GameMigong::startGame);
}

GameMigong::~GameMigong()
{
}

void GameMigong::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 绘制小人
    painter.drawPixmap(player, playerPixmap);

    // 绘制目标
    painter.drawPixmap(target, targetPixmap);

    // 绘制障碍物
    for (const QRect &obstacle : obstacles) {
        painter.drawPixmap(obstacle, obstaclePixmap);
    }
}

void GameMigong::keyPressEvent(QKeyEvent *event)
{
    if (isGameStarted) {
        if (event->key() == Qt::Key_Up) {
            playerDirection = QPoint(0, -1);
        } else if (event->key() == Qt::Key_Down) {
            playerDirection = QPoint(0, 1);
        } else if (event->key() == Qt::Key_Left) {
            playerDirection = QPoint(-1, 0);
        } else if (event->key() == Qt::Key_Right) {
            playerDirection = QPoint(1, 0);
        }
    }
}

void GameMigong::movePlayer()
{
    if (isGameStarted && !isGameOver) {
        player.moveTopLeft(player.topLeft() + playerDirection * moveStep);
        checkButtonCollisions();
        checkCollisions();

        if (player.intersects(target)) {
            isGameOver = true;
            showSuccessMessage();
            if (currentLevel == 1) {
                currentLevel = 2;  // 切换到第二关
                loadLevel(currentLevel);  // 加载第二关
            }
        }

        update();
    }
}

void GameMigong::checkCollisions()
{
    for (const QRect &obstacle : obstacles) {
        if (player.intersects(obstacle)) {
            isGameOver = true;
            break;
        }
    }
}

void GameMigong::checkButtonCollisions()
{
    for (DraggableButton *button : buttons) {
        if (player.intersects(button->geometry())) {
            if (button->geometry().contains(player.center())) {
                if (button->buttonText == "Up") {
                    playerDirection = QPoint(0, -1);
                } else if (button->buttonText == "Down") {
                    playerDirection = QPoint(0, 1);
                } else if (button->buttonText == "Left") {
                    playerDirection = QPoint(-1, 0);
                } else if (button->buttonText == "Right") {
                    playerDirection = QPoint(1, 0);
                }
            }
        }
    }
}

void GameMigong::spawnButtons()
{
    buttons.clear();

    if (currentLevel == 1) {
        // 第一关设置
        DraggableButton *upButton = new DraggableButton(this);
        upButton->setGeometry(100, 100, 50, 50);
        upButton->setText("Up");

        DraggableButton *rightButton = new DraggableButton(this);
        rightButton->setGeometry(200, 100, 50, 50);
        rightButton->setText("Right");

        DraggableButton *downButton = new DraggableButton(this);
        downButton->setGeometry(300, 100, 50, 50);
        downButton->setText("Down");

        buttons.append(upButton);
        buttons.append(rightButton);
        buttons.append(downButton);
    } else if (currentLevel == 2) {
        // 第二关设置
        DraggableButton *upButton = new DraggableButton(this);
        upButton->setGeometry(100, 100, 50, 50);
        upButton->setText("Up");

        DraggableButton *rightButton1 = new DraggableButton(this);
        rightButton1->setGeometry(200, 100, 50, 50);
        rightButton1->setText("Right");

        DraggableButton *rightButton2 = new DraggableButton(this);
        rightButton2->setGeometry(300, 100, 50, 50);
        rightButton2->setText("Right");

        DraggableButton *leftButton1 = new DraggableButton(this);
        leftButton1->setGeometry(400, 100, 50, 50);
        leftButton1->setText("Left");

        DraggableButton *leftButton2 = new DraggableButton(this);
        leftButton2->setGeometry(500, 100, 50, 50);
        leftButton2->setText("Left");

        DraggableButton *downButton = new DraggableButton(this);
        downButton->setGeometry(600, 100, 50, 50);
        downButton->setText("Down");

        buttons.append(upButton);
        buttons.append(rightButton1);
        buttons.append(rightButton2);
        buttons.append(leftButton1);
        buttons.append(leftButton2);
        buttons.append(downButton);
    }
}

void GameMigong::loadLevel(int level)
{
    // 根据关卡加载不同的障碍物、方向块和图像
    if (level == 1) {
        obstacles.clear();
        obstacles.append(QRect(200, height() / 2 - 50, 50, 50));  // 第一关的障碍物
        spawnButtons();
        playerPixmap = QPixmap(":/images/src/migongcharacter.jpg");
        obstaclePixmap = QPixmap(":/images/obstacle1.png");
        targetPixmap = QPixmap(":/images/target1.png");
    } else if (level == 2) {
        obstacles.clear();
        obstacles.append(QRect(200, height() / 2 - 50, 50, 50));
        obstacles.append(QRect(300, height() / 2, 50, 50));
        obstacles.append(QRect(400, height() / 2 - 50, 50, 50));
        obstacles.append(QRect(500, height() / 2, 50, 50));
        spawnButtons();
        playerPixmap = QPixmap("::/images/src/character.png");
        obstaclePixmap = QPixmap(":/images/obstacle2.png");
        targetPixmap = QPixmap(":/images/target2.png");
    }
}

void GameMigong::startGame()
{
    isGameStarted = true;
    for (DraggableButton *button : buttons) {
        button->setDraggable(false);
    }
}

void GameMigong::showSuccessMessage()
{
    QMessageBox::information(this, "游戏成功", "恭喜你，成功到达终点！");
}
