#ifndef MIGONGGAME_H
#define MIGONGGAME_H

#include <QWidget>
#include <QRect>
#include <QPixmap>
#include <QVector>

class DraggableButton : public QWidget
{
    Q_OBJECT

public:
    explicit DraggableButton(QWidget *parent = nullptr);
    ~DraggableButton();

    bool isDragging;
    bool draggable;
    QPixmap buttonImage;

    void setImage(const QPixmap &image);  // 设置按钮图像
    QPixmap getButtonImage() const;  // 获取按钮图像

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:

    QPoint dragStartPos;
};

class GameMigong : public QWidget
{
    Q_OBJECT

public:
    explicit GameMigong(QWidget *parent = nullptr);
    ~GameMigong();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void movePlayer();
    void checkCollisions();
    void checkButtonCollisions();
    void loadLevel(int level);
    void spawnButtons(int level);
    void startGame();
    void restart();
    void showSuccessMessage();
    void startNewLevel(int level);
    void setLevel(int level);
    void buttonposition();
    QRect player;
    QPoint playerDirection;
    QPixmap playerPixmap;
    QPixmap obstaclePixmap;
    QPixmap targetPixmap;
    QRect target;
    QVector<QRect> obstacles;
    QVector<DraggableButton *> buttons;
    QTimer *timer;
    bool isGameOver;
    bool isGameStarted;
    int moveStep;
    int currentLevel;
};

#endif // MIGONGGAME_H
