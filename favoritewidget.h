#ifndef FAVORITEWIDGET_H
#define FAVORITEWIDGET_H

#include <QWidget>
#include "nodelist.h"

namespace Ui {
class FavoriteWidget;
}

//自定义窗口类
class FavoriteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteWidget(QWidget *parent = nullptr);
    ~FavoriteWidget();
    void listRunTest();
    void loadFavoriteList();

private slots://槽函数的定义为 private 是为了封装它们，确保只有 Qt 的信号机制可以触发这些槽函数。
    //声明槽函数
    void onRatingChanged(int value);
    void showAddWidget();
    void showDeleteWidget();

    void handleSubmitAction();
    void handleDeleteAction();
    void handleExitAction();
    void handleExit2Action();
    void handleExit2_2Action();


private:
    Ui::FavoriteWidget *ui;
    NodeList favoriteList;

    void addFavoriteByName(const QString &name);
    void deleteFavoriteByName(const QString &name);
    void updateFavoriteDisplay();
    void saveFavoriteList();

};

#endif // FAVORITEWIDGET_H
