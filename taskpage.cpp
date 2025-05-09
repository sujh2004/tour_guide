#include "taskpage.h"
#include <QPainter>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QRegularExpression>
#include <QShowEvent>       // 记得头文件

TaskPage::TaskPage(QWidget *parent)
    : QDialog(parent)
{
    /* —— 固定窗口 —— */
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(kWidth, kHeight);

    /* —— 场景 / 视图 —— */
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, kWidth, kHeight);

    view  = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, kWidth, kHeight);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFrameShape(QFrame::NoFrame);

    /* —— 按钮 —— */
    btnShot = new QPushButton(tr("拍照"), this);
    btnBack = new QPushButton(tr("返回"), this);

    btnShot->resize(80, 32);
    btnBack->resize(80, 32);

    btnShot->move(width() - btnShot->width() - 20,
                  height() - btnShot->height() - 15);
    btnBack->move(20,
                  height() - btnBack->height() - 15);

    btnShot->raise();   // 覆盖在 view 上层
    btnBack->raise();

    connect(btnShot, &QPushButton::clicked,
            this,     &TaskPage::onShotClicked);
    connect(btnBack, &QPushButton::clicked,
            this,     &TaskPage::onBackClicked);
}

/* —— 设置场景名，供截图命名 —— */
void TaskPage::setSceneInfo(const QString &name)
{
    sceneName = name.trimmed();   // 去掉首尾空格防止空文件名
}

/* ——— 背景图 ——— */
void TaskPage::setBuildingImage(const QString &imagePath)
{
    sceneName = QFileInfo(imagePath).baseName();

    QPixmap pix(imagePath);
    if (pix.isNull()) { qWarning() << "加载背景图失败:" << imagePath; return; }

    pix = pix.scaled(kWidth, kHeight,
                     Qt::KeepAspectRatioByExpanding,
                     Qt::SmoothTransformation);

    scene->clear();
    auto *bgItem = scene->addPixmap(pix);
    bgItem->setZValue(-1);

    /* 把小人重新放回场景 */
    if (playerItem) {
        scene->addItem(playerItem);
        playerItem->setZValue(1);
        playerItem->setPos((kWidth  - playerItem->boundingRect().width()) / 2,
                           (kHeight - playerItem->boundingRect().height()) / 2);
    }

    /* -------- 关键新增 -------- */
    //view->resetTransform();                                  // 清掉旧矩阵
    //view->fitInView(scene->sceneRect(),
    //                Qt::KeepAspectRatioByExpanding);         // 立即把图铺满
}

/* ——— 小人 ——— */
void TaskPage::setPlayerImage(const QString &imagePath, qreal scale)
{
    QPixmap pix(imagePath);
    if (pix.isNull()) return;
    if (scale != 1.0)
        pix = pix.scaled(pix.size()*scale,
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);

    if (!playerItem)
        playerItem = scene->addPixmap(pix);
    else
        playerItem->setPixmap(pix);

    playerItem->setZValue(1);
    playerItem->setPos((kWidth  - playerItem->boundingRect().width())/2,
                       (kHeight - playerItem->boundingRect().height())/2);
}

/* ——— 保持铺满 ——— */
void TaskPage::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

/* ——— WSAD 控制小人 ——— */
void TaskPage::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W: playerItem->moveBy(0, -10); break;
    case Qt::Key_S: playerItem->moveBy(0,  10); break;
    case Qt::Key_A: playerItem->moveBy(-10, 0); break;
    case Qt::Key_D: playerItem->moveBy( 10, 0); break;
    default:        QDialog::keyPressEvent(event);
    }
}
/* ——— 窗口真正显示的时刻 ——— */
void TaskPage::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    /* 这时 viewport 已经是 800×500，重新铺满一次 */
    view->resetTransform();
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

/* ——— 截图 ——— */
void TaskPage::onShotClicked()
{
    QDir dir("./screenshots");
    if (!dir.exists()) dir.mkpath(".");

    QString safeName = sceneName.isEmpty() ? "screenshot" : sceneName;
    safeName.replace(QRegularExpression(R"([\\/:*?"<>|])"), "_");

    QString file = dir.filePath(QString("%1_%2.png")
                                    .arg(safeName)
                                    .arg(QDateTime::currentDateTime()
                                             .toString("yyyyMMdd_HHmmss")));

    /* 直接渲染 scene ⇒ 只会截取 800×500 的背景和小人 */
    QImage img(kWidth, kHeight, QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter p(&img);
    scene->render(&p);
    img.save(file);

    QMessageBox::information(this, "已保存",
                             "截图已保存到背包：\n" +
                                 QDir::toNativeSeparators(file));
}

/* ——— 返回 ——— */
void TaskPage::onBackClicked()
{
    emit backToMain();
    accept();
}
