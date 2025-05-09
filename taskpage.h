#ifndef TASKPAGE_H
#define TASKPAGE_H

#include <QDialog>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QKeyEvent>

class TaskPage : public QDialog
{
    Q_OBJECT
public:
    explicit TaskPage(QWidget *parent = nullptr);

    void setBuildingImage(const QString &imagePath);                // 背景
    void setPlayerImage(const QString &imagePath, qreal scale = 1); // 小人
    void setSceneInfo(const QString &name);

signals:
    void backToMain();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *e)    override;

    void showEvent(QShowEvent *event) override;   // <— 新增


private slots:
    void onShotClicked();
    void onBackClicked();

private:
    static constexpr int kWidth  = 800;
    static constexpr int kHeight = 500;

    QString               sceneName;
    QGraphicsView        *view        {nullptr};
    QGraphicsScene       *scene       {nullptr};
    QGraphicsPixmapItem  *playerItem  {nullptr};
    QPushButton          *btnShot     {nullptr};
    QPushButton          *btnBack     {nullptr};
};

#endif // TASKPAGE_H
