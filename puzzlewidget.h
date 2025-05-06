#ifndef PUZZLEWIDGET_H
#define PUZZLEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QVector>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QMessageBox>
#include <QPushButton>

class PuzzleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PuzzleWidget(QWidget *parent = nullptr);
    ~PuzzleWidget();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int puzzleWidth;  // 拼图的宽度
    int puzzleHeight;  // 拼图的高度
    int rows;  // 拼图的行数
    int cols;  // 拼图的列数

    int puzzlePieceWidth;  // 每个拼图块的宽度
    int puzzlePieceHeight;  // 每个拼图块的高度

    QVector<QImage> puzzlePieces;  // 存储拼图块
    QVector<QImage> originalPieces;  // 存储原始拼图块的顺序
    bool isPieceDragged;  // 是否正在拖动拼图块
    int draggedPieceIndex;  // 正在拖动的拼图块的索引
    QPoint draggedPiecePos;  // 被拖动拼图块的相对位置

    // 添加按钮对象
    QPushButton* button1;
    QPushButton* button2;
    QPushButton* button3;

    // 添加槽函数
    void loadPuzzle1();
    void loadPuzzle2();
    void loadPuzzle3();
    void loadImage(const QString& imagePath);  // 加载图像并切割成拼图块

    void shufflePuzzlePieces();  // 随机打乱拼图块
    void checkPuzzleCompletion();  // 检查拼图是否完成
};

#endif // PUZZLEWIDGET_H
