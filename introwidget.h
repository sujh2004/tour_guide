#ifndef INTROWIDGET_H
#define INTROWIDGET_H
#include"Node.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QWidget>
#include <QTimer>

namespace Ui {
class IntroWidget;
}

class IntroWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IntroWidget(QWidget *parent = nullptr);
    ~IntroWidget();
    void showIntro(const Node &node);
    void hideIntro();
private:
    Ui::IntroWidget *ui;
    QString typeToString(Type type);

    //fade- in and out ani
    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *fadeAnimation;


};

#endif // INTROWIDGET_H
