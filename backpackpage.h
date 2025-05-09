#ifndef BACKPACKPAGE_H
#define BACKPACKPAGE_H

#include <QDialog>
#include <QGridLayout>
#include <QDir>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QMessageBox>

/* 可点击缩略图 */
class ClickableLabel : public QLabel {
    Q_OBJECT
public:
    explicit ClickableLabel(const QString &path, QWidget *parent=nullptr)
        : QLabel(parent), imgPath(path) {}
signals:
    void clicked(const QString &path);
protected:
    void mousePressEvent(QMouseEvent*) override { emit clicked(imgPath); }
private:
    QString imgPath;
};

/* 背包弹窗 */
class BackpackPage : public QDialog {
    Q_OBJECT
public:
    explicit BackpackPage(QWidget *parent=nullptr);
private:
    static constexpr int CARD_W = 180;     /*** 新增：卡片固定宽高 ***/
    static constexpr int CARD_H = 230;

    void loadImages();                       // 读取 ./screenshots
    void addThumb(const QString &filePath, int row, int col);

    QScrollArea   *scrollArea {nullptr};
    QWidget       *container  {nullptr};
    QGridLayout   *grid       {nullptr};
};

#endif // BACKPACKPAGE_H
