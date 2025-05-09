#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QKeyEvent>
#include <QDir>
#include <QDateTime>

class TaskPage : public QDialog
{
    Q_OBJECT
public:
    explicit TaskPage(QWidget *parent = nullptr);

    /** 设置背景图（终点建筑） */
    void setBuildingImage(const QString &imagePath);
    /** 设置小人图；默认按 0.05 倍缩放 */
    void setPlayerImage(const QString &imagePath, qreal scale = 0.05);
    /** 把景点名传进来，用于拼接截图文件名 */
    void setSceneInfo(const QString &name);

signals:
    void backToMain();               // 通知主界面返回

protected:
    void keyPressEvent(QKeyEvent *event) override;   // WSAD 控制小人

private slots:
    void onShotClicked();            // 拍照
    void onBackClicked();            // 返回

private:
    QString               sceneName;
    QGraphicsView        *view        {nullptr};
    QGraphicsScene       *scene       {nullptr};
    QGraphicsPixmapItem  *playerItem  {nullptr};
    QPushButton          *btnShot     {nullptr};
    QPushButton          *btnBack     {nullptr};
};

#endif // TASKPAGE_H
