#ifndef GAMEMIGONG_H
#define GAMEMIGONG_H

#include <QWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include <QMouseEvent>
#include <QPushButton>
#include <QMessageBox>

class DraggableButton : public QWidget
{
    Q_OBJECT

public:
    explicit DraggableButton(QWidget *parent = nullptr);
    ~DraggableButton();
    void setText(const QString &text);
    void setDraggable(bool draggable);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    QString buttonText;

private:
    bool isDragging;
    QPoint dragStartPos;
    bool draggable;
};

class GameMigong : public QWidget
{
    Q_OBJECT

public:
    explicit GameMigong(QWidget *parent = nullptr);
    ~GameMigong();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void movePlayer();
    void checkCollisions();
    void checkButtonCollisions();
    void spawnButtons();
    void startGame();
    void showSuccessMessage();
    void loadLevel(int level);  // 加载指定关卡

    QRect player;
    QPoint playerDirection;
    QList<DraggableButton*> buttons;
    QRect target;
    QList<QRect> obstacles;
    bool isGameOver;
    bool isGameStarted;
    int moveStep;
    int currentLevel;  // 当前关卡
    QTimer *timer;

    QPixmap playerPixmap;  // 小人图像
    QPixmap obstaclePixmap;  // 障碍物图像
    QPixmap targetPixmap;  // 终点图像
};

#endif // GAMEMIGONG_H
