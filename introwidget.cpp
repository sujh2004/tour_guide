#include "introwidget.h"
#include "ui_introwidget.h"
#include "node.h"

IntroWidget::IntroWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IntroWidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);

}

IntroWidget::~IntroWidget()
{
    delete ui;
}

QString IntroWidget::typeToString(Type type) {
    switch (type) {
    case BUILDING: return "建筑";
    case INTERSECTION: return "路口";
    case TOLIET: return "厕所";
    case RESTERANT: return "饭店";
    default: return "未知";
    }
}

void IntroWidget::showIntro(const Node &node) {
    // 设置文字
    ui->description->setText(QString("景点描述： %1").arg(node.getDescription()));
    ui->id->setText(QString("景点ID:%1").arg(QString::number(node.getId())));
    ui->name->setText(node.getName());
    ui->type->setText(QString("景点类型：%1").arg(typeToString(node.getType())));
    // 设置背景图片（方式一：StyleSheet）
    QString style = QString(
                        "QLabel {"
                        "border: 2px solid #ccc;"
                        "border-radius: 8px;"
                        "border-image: url(%1);"
                        "background-position: center;"
                        "background-repeat: no-repeat;"
                        "}").arg(node.getImagePath());

    ui->img->setStyleSheet(style);

    // 使用 QPropertyAnimation 进行淡入动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);  // 300毫秒
    animation->setStartValue(0.0);  // 从透明开始
    animation->setEndValue(1.0);    // 结束为不透明
    animation->start();

    this->show();
}

void IntroWidget::hideIntro() {
    // 使用 QPropertyAnimation 进行淡出动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);  // 300毫秒
    animation->setStartValue(1.0);  // 从不透明开始
    animation->setEndValue(0.0);    // 结束为透明
    animation->start();

    // 延迟后隐藏窗口
    QTimer::singleShot(300, this, &IntroWidget::hide);
}
