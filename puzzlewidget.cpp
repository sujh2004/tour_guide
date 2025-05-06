#include "puzzlewidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPushButton>
#include <QVBoxLayout>

PuzzleWidget::PuzzleWidget(QWidget *parent)
    : QWidget(parent),
    puzzleWidth(600), puzzleHeight(900), rows(3), cols(3),
    puzzlePieceWidth(puzzleWidth / cols), puzzlePieceHeight(puzzleHeight / rows),
    isPieceDragged(false), draggedPieceIndex(-1)
{
    // 设置窗口大小为 600x900
    setFixedSize(puzzleWidth, puzzleHeight);

    // 创建按钮并设置位置
    QPushButton* button1 = new QPushButton("拼图1", this);
    QPushButton* button2 = new QPushButton("拼图2", this);
    QPushButton* button3 = new QPushButton("拼图3", this);

    button1->setGeometry(500, 650, 100, 50);  // 设置按钮位置
    button2->setGeometry(500, 750, 100, 50);
    button3->setGeometry(500, 850, 100, 50);

    // 连接按钮点击事件
    connect(button1, &QPushButton::clicked, this, &PuzzleWidget::loadPuzzle1);
    connect(button2, &QPushButton::clicked, this, &PuzzleWidget::loadPuzzle2);
    connect(button3, &QPushButton::clicked, this, &PuzzleWidget::loadPuzzle3);

    // 默认加载拼图1
    loadPuzzle1();
}

PuzzleWidget::~PuzzleWidget() {
    // 清理拼图块或其他资源
    puzzlePieces.clear();
    originalPieces.clear();
}

void PuzzleWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // 绘制每个拼图块
    for (int i = 0; i < puzzlePieces.size(); ++i) {
        int row = i / cols;
        int col = i % cols;
        int x = col * puzzlePieceWidth;
        int y = row * puzzlePieceHeight;

        // 如果是拖动中的拼图块，需要特别处理绘制位置
        if (isPieceDragged && i == draggedPieceIndex) {
            QPoint newPos = mapFromGlobal(QCursor::pos()) - draggedPiecePos;  // 获取鼠标当前位置并更新拼图块的位置
            painter.drawImage(newPos.x(), newPos.y(), puzzlePieces[i]);
        } else {
            painter.drawImage(x, y, puzzlePieces[i]);
        }
    }

    QWidget::paintEvent(event);
}

void PuzzleWidget::mousePressEvent(QMouseEvent *event) {
    // 确保拖动的拼图块索引有效
    for (int i = 0; i < puzzlePieces.size(); ++i) {
        int row = i / cols;
        int col = i % cols;
        int x = col * puzzlePieceWidth;
        int y = row * puzzlePieceHeight;

        QRect pieceRect(x, y, puzzlePieceWidth, puzzlePieceHeight);
        if (pieceRect.contains(event->pos())) {
            draggedPieceIndex = i;  // 记录正在拖动的拼图块的索引
            isPieceDragged = true;
            draggedPiecePos = event->pos() - pieceRect.topLeft();  // 记录鼠标点击相对拼图块的位置
            break;
        }
    }
}

void PuzzleWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isPieceDragged && draggedPieceIndex >= 0 && draggedPieceIndex < puzzlePieces.size()) {
        update();  // 更新显示，移动拼图块
    }
}

void PuzzleWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (isPieceDragged && draggedPieceIndex >= 0 && draggedPieceIndex < puzzlePieces.size()) {
        // 获取鼠标释放时的坐标
        int releaseX = event->pos().x();
        int releaseY = event->pos().y();

        // 遍历拼图块，检查鼠标释放的位置是否在拼图块内
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                // 计算当前拼图块的位置
                int x = col * puzzlePieceWidth;
                int y = row * puzzlePieceHeight;

                // 检查鼠标释放位置是否在当前拼图块的区域内
                if (releaseX >= x && releaseX < x + puzzlePieceWidth && releaseY >= y && releaseY < y + puzzlePieceHeight) {
                    // 如果鼠标释放在拼图块内，计算目标拼图块的索引
                    int targetIndex = row * cols + col;

                    // 如果目标拼图块不同于当前拖动的拼图块，交换它们
                    if (targetIndex != draggedPieceIndex) {
                        std::swap(puzzlePieces[draggedPieceIndex], puzzlePieces[targetIndex]);
                    }

                    // 停止拖动
                    isPieceDragged = false;
                    update();  // 更新显示
                    checkPuzzleCompletion();  // 检查拼图是否完成
                    return;  // 找到目标拼图块后，退出
                }
            }
        }
    }
}

void PuzzleWidget::shufflePuzzlePieces() {
    // 随机打乱拼图块
    int n = puzzlePieces.size();
    for (int i = 0; i < n; ++i) {
        int randomIndex = QRandomGenerator::global()->bounded(n);  // 生成随机数
        std::swap(puzzlePieces[i], puzzlePieces[randomIndex]);  // 交换拼图块
    }
}

void PuzzleWidget::checkPuzzleCompletion() {
    bool isComplete = true;
    for (int i = 0; i < puzzlePieces.size(); ++i) {
        if (puzzlePieces[i] != originalPieces[i]) {
            isComplete = false;
            break;
        }
    }

    if (isComplete) {
        QMessageBox::information(this, "拼图完成", "恭喜！你完成了拼图！");
    }
}

void PuzzleWidget::loadPuzzle1() {
    loadImage(":/images/src/wenwu1.jpg");
}

void PuzzleWidget::loadPuzzle2() {
    loadImage(":/images/src/wenwu2.jpg");
}

void PuzzleWidget::loadPuzzle3() {
    loadImage(":/images/src/wenwu3.jpg");
}

void PuzzleWidget::loadImage(const QString& imagePath) {
    // 加载新的图片
    QImage originalImage(imagePath);
    if (originalImage.isNull()) {
        qWarning() << "Image not found!";
    }

    // 清空之前的拼图块
    puzzlePieces.clear();

    // 将新图像分割成多个拼图块
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            QRect rect(col * puzzlePieceWidth, row * puzzlePieceHeight, puzzlePieceWidth, puzzlePieceHeight);
            puzzlePieces.push_back(originalImage.copy(rect));  // 切割并存储每个拼图块
        }
    }

    // 保存原始拼图块的顺序
    originalPieces = puzzlePieces;

    // 打乱拼图块顺序
    shufflePuzzlePieces();

    update();  // 更新显示
}
