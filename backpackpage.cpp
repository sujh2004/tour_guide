#include "backpackpage.h"
#include <QVBoxLayout>
#include <QDateTime>

BackpackPage::BackpackPage(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("背包 · 我的留影");
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setAttribute(Qt::WA_DeleteOnClose);
    resize(900, 550);

    /*— 古风底色与字体 —*/
    setStyleSheet("QDialog{background-image:url(:/ui/paper.jpg);}"
                  "QLabel{font-family:'KaiTi';color:#4A2E1B;}"
                  "QPushButton{font-family:'KaiTi';background:#C8B5B3;"
                  "border-radius:6px;padding:2px 8px;color:#4A2E1B;}");

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    container  = new QWidget(scrollArea);
    grid       = new QGridLayout(container);
    grid->setHorizontalSpacing(8);                  /*** 更紧凑 ***/
    grid->setVerticalSpacing  (12);
    grid->setContentsMargins  (12,12,12,12);
     grid->setAlignment(Qt::AlignTop | Qt::AlignLeft);/*** 改成左上对齐 ***/
    grid->setSpacing(12);
    scrollArea->setWidget(container);

    QVBoxLayout *dlgLay = new QVBoxLayout(this);
    dlgLay->addWidget(scrollArea);

    loadImages();
}

/* ------------- 私有函数 ------------- */
void BackpackPage::loadImages()
{
    /* 先清空旧控件 */
    QLayoutItem *child;
    while ((child = grid->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QDir dir("./screenshots");
    QStringList files = dir.entryList(QStringList() << "*.png" << "*.jpg",
                                      QDir::Files, QDir::Time);   // 新→旧

    const int COL_MAX = 4;
    int row = 0, col = 0;

    for (const QString &f : files) {
        addThumb(dir.filePath(f), row, col);
        if (++col >= COL_MAX) { col = 0; ++row; }
    }
}



void BackpackPage::addThumb(const QString &filePath, int row, int col)
{
    QPixmap pix(filePath);

    /* 外层卡片 */
    QWidget *card = new QWidget(container);
    card->setFixedSize(CARD_W, CARD_H);             /*** 关键：锁高度 ***/
    card->setStyleSheet("background:#F9F6F2; border:1px solid #928187;"
                        "border-radius:4px;");

    QVBoxLayout *v = new QVBoxLayout(card);
    v->setSpacing(3);
    v->setContentsMargins(4,4,4,4);

    /* 缩略图 */
    ClickableLabel *thumb = new ClickableLabel(filePath, card);
    thumb->setPixmap(pix.scaled(170, 170, Qt::KeepAspectRatio,
                                Qt::SmoothTransformation));
    thumb->setAlignment(Qt::AlignCenter);
    v->addWidget(thumb, 0, Qt::AlignCenter);

    /* 景点名 */
    QString scenic = QFileInfo(filePath).baseName().section('_', 0, 0);
    QLabel *nameLbl = new QLabel(scenic, card);
    nameLbl->setAlignment(Qt::AlignCenter);
    nameLbl->setFixedHeight(20);
    v->addWidget(nameLbl);

    /* 删除按钮 */
    QPushButton *btnDel = new QPushButton("删除", card);
    btnDel->setFixedWidth(46);
    v->addWidget(btnDel, 0, Qt::AlignCenter);
    grid->addWidget(card, row, col);

    /* —— 信号槽 —— */
    connect(thumb, &ClickableLabel::clicked, this, [=](const QString &path){
        QDialog dlg(this);
        dlg.setWindowTitle(scenic);
        dlg.resize(900, 600);
        QLabel big(&dlg);
        big.setPixmap(QPixmap(path).scaled(dlg.size(),
                                           Qt::KeepAspectRatio,
                                           Qt::SmoothTransformation));
        big.setAlignment(Qt::AlignCenter);
        QVBoxLayout lay(&dlg); lay.addWidget(&big);
        dlg.exec();
    });

    connect(btnDel, &QPushButton::clicked, this, [=]{
        if (QMessageBox::question(this, "确认删除",
                                  "确定要删除这张照片吗？") == QMessageBox::Yes) {
            QFile::remove(filePath);
            loadImages();          // 立即刷新
        }
    });

    grid->addWidget(card, row, col);
}
