#include "taskpage.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QRegularExpression>   // ★ 新增：用正则替换非法字符


TaskPage::TaskPage(QWidget *parent)
    : QDialog(parent)
{
    /*--- 基本窗口属性 ---*/
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setAttribute(Qt::WA_DeleteOnClose);    // 关闭即自动回收
    setFixedSize(800, 450);                // 弹窗统一尺寸

    /*--- 场景 / 视图 ---*/
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width(), height());    // 锁死矩形

    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOff);

    /*--- 按钮 ---*/
    btnShot = new QPushButton("拍照", this);
    connect(btnShot, &QPushButton::clicked, this, &TaskPage::onShotClicked);

    btnBack = new QPushButton("返回", this);
    connect(btnBack, &QPushButton::clicked, this, &TaskPage::onBackClicked);

    /*--- 布局 ---*/
    QVBoxLayout *lay = new QVBoxLayout(this);
    lay->addWidget(view, 1);
    lay->addWidget(btnShot);
    lay->addWidget(btnBack);
}

/* 背景图 */
void TaskPage::setBuildingImage(const QString &imagePath)
{
    QPixmap pix(imagePath);
    scene->clear();
    scene->setSceneRect(0, 0, width(), height());
    scene->addPixmap(pix.scaled(size(),
                                Qt::KeepAspectRatioByExpanding,
                                Qt::SmoothTransformation))
        ->setZValue(-1);                     // 背景置底
    if (playerItem) scene->addItem(playerItem);
}

/* 小人图 */
void TaskPage::setPlayerImage(const QString &imagePath, qreal scale)
{
    QPixmap pix(imagePath);
    if (scale != 1.0)
        pix = pix.scaled(pix.size() * scale,
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);

    if (!playerItem) {
        playerItem = scene->addPixmap(pix);
        playerItem->setZValue(1);
        // 居中放
        playerItem->setPos(width()/2 - playerItem->boundingRect().width()/2,
                           height()/2 - playerItem->boundingRect().height()/2);
    } else {
        playerItem->setPixmap(pix);
    }
}

/* 保存景点名，拼截图文件名用 */
void TaskPage::setSceneInfo(const QString &name)
{
    sceneName = name;
}

/* WSAD 控制小人移动 */
void TaskPage::keyPressEvent(QKeyEvent *event)
{
    if (!playerItem) { QDialog::keyPressEvent(event); return; }

    switch (event->key()) {
    case Qt::Key_W: playerItem->moveBy(0, -10); break;
    case Qt::Key_S: playerItem->moveBy(0,  10); break;
    case Qt::Key_A: playerItem->moveBy(-10, 0); break;
    case Qt::Key_D: playerItem->moveBy( 10, 0); break;
    default:        QDialog::keyPressEvent(event);
    }
}

/* 拍照 */
void TaskPage::onShotClicked()
{
    QDir dir("./screenshots");
    if (!dir.exists()) dir.mkpath(".");

    QString safeName = sceneName.isEmpty() ? "screenshot"
                                           : sceneName;   /*** 修改 ***/
    /* 把文件名中的 ‘/ \ : * ? " < > |’ 等非法字符都替换成 '_' */
    safeName.replace(QRegularExpression(R"([\\/:*?"<>|])"), "_");  /*** 新增 ***/

    QString filePath = dir.filePath(QString("%1_%2.png")
                                        .arg(safeName)
                                        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")));
    grab().save(filePath);
    QMessageBox::information(this, "已保存",
                             "截图已保存到背包：\n"
                                 + QDir::toNativeSeparators(filePath));
}


/* 返回 */
void TaskPage::onBackClicked()
{
    emit backToMain();
    accept();               // 关闭对话框
}
