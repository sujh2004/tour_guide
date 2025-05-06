#include "favoritewidget.h"
#include "ui_favoritewidget.h"
#include "nodelist.h"
#include "mainwindow.h"
#include <QSettings>>

//构造函数初始化列表
FavoriteWidget::FavoriteWidget(QWidget *parent)
    : QWidget(parent)//调用基类 QWidget 的构造函数，传入父指针
    , ui(new Ui::FavoriteWidget)// Ui::FavoriteWidget 实例
{
    ui->setupUi(this);//把 .ui 文件中设计的界面加载到当前这个 FavoriteWidget 对象上
    this->setAttribute(Qt::WA_StyledBackground, true);
    ui->addWidget->hide();//先把添加景点窗口hide
    ui->deleteWidget->hide();
    ui->ratingSlider->setMinimum(0);
    ui->ratingSlider->setMaximum(50);
    ui->ratingSlider->setSingleStep(1);//设slider步长为1
    ui->ratingSlider->setValue(30);//初始3.0星
    ui->ratingLabel->setText("喜好程度：3.0");


    //链接ratingLbel & Slider,connect(信號發出者, 信號, 接收者, 槽函數);senderClass::signal ReceiverClass::
    //For绑定slider和ratingLabel
    connect(ui->ratingSlider,&QSlider::valueChanged,this,&FavoriteWidget::onRatingChanged);//槽函数传参直接用信号，valuechanged传回当前滑动条值
    //For addButton触发addWidget show
    connect(ui->addButton,&QPushButton::clicked,this,&FavoriteWidget::showAddWidget);
    //For 提交"增加景点按钮"
    connect(ui->subButton,&QPushButton::clicked,this,&FavoriteWidget::handleSubmitAction);
    //For退出favoriteWidget
    connect(ui->exitButton,&QPushButton::clicked,this,&FavoriteWidget::handleExitAction);
    //For退出addWidget
    connect(ui->exitButton2,&QPushButton::clicked,this,&FavoriteWidget::handleExit2Action);
    //For delButton触发deleteWidget show
    connect(ui->delButton,&QPushButton::clicked,this,&FavoriteWidget::showDeleteWidget);
    //For deleteWidget里的"删除收藏按钮"
    connect(ui->deleteButton,&QPushButton::clicked,this,&FavoriteWidget::handleDeleteAction);
    //For 退出deleteWidget
    connect(ui->exitButton2_2,&QPushButton::clicked,this,&FavoriteWidget::handleExit2_2Action);
    //test linkedList
    // listRunTest();
}

FavoriteWidget::~FavoriteWidget()
{
    delete ui;
}

//测试链表功能
void FavoriteWidget::listRunTest(){
    //--------删除功能test BEGIN-------
    // NodeList myList;

    // //创建node
    // Node *a = new Node(1,0,0,"天安门",BUILDING);
    // a->setRating(4.5);
    // Node* b = new Node(2, 0, 0, "午门", BUILDING);
    // b->setRating(3.0);
    // Node* c = new Node(3, 0, 0, "太和殿", BUILDING);
    // c->setRating(5.0);

    // //在list里面加入Node
    // myList.addNode(a);
    // myList.addNode(b);
    // myList.addNode(c);

    // // 打印链表
    // myList.displayList(); // 按照评分顺序输出

    // // 删除节点并重新打印
    // myList.deleteNodeByName("午门");
    // qDebug() << "删除后：";
    // myList.displayList();

    //--------删除功能test END-------

    //--------排序功能test BEGIN-------
    NodeList list;
    list.addNode(new Node(1, 0, 0, "故宫", BUILDING, 4.5));
    list.addNode(new Node(2, 0, 0, "天安门", BUILDING, 3.7));
    list.addNode(new Node(3, 0, 0, "长城", BUILDING, 4.5));
    list.addNode(new Node(4, 0, 0, "颐和园", BUILDING, 5.0));
    list.addNode(new Node(5, 0, 0, "鸟巢", BUILDING, 3.7));

    qDebug() << "排序前：";
    list.displayList();

    list.sortListByRating(); // 调用归并排序

    qDebug() << "排序后：";
    list.displayList();

}

void FavoriteWidget::onRatingChanged(int value){
    double rating = value / 10.0;
    ui->ratingLabel->setText(QString("喜好程度：%1").arg(QString::number(rating, 'f', 1)));
}


void FavoriteWidget::handleSubmitAction(){
    //找到子窗口addWidget的placeInput
    QLineEdit* nameLineEdit = ui->addWidget->findChild<QLineEdit*>("placeInput");
    if (nameLineEdit) {
        QString name = nameLineEdit->text();  // 获取 QLineEdit 中的文本
        addFavoriteByName(name);  // 查找并添加景点
        updateFavoriteDisplay();
        nameLineEdit->clear();
        ui->addWidget->hide();   // 隐藏 addWidget
    } else {
        qDebug() << "placeInput 控件未找到！";
    }
}

// 查找景点并添加到收藏列表中
void FavoriteWidget::addFavoriteByName(const QString &name)
{
    if (!parent()) {
        qDebug() << "父对象为空！";
        return;
    }

    //获取父对象来访问nodes
    MainWindow* mw = qobject_cast<MainWindow*>(parent());
    if (!mw) {
        qDebug() << "父对象不是 MainWindow类。";
        return;
    }

    NodeList* nodeList = mw->getNodes(); // 获取景点列表
    if (!nodeList) {
        qDebug() << "获取景点节点列表失败。";
        return;
    }

    Node* current = nodeList->getHead();
    bool found = false;  // 用于标记是否找到景点

    while (current) {
        if (current->getName() == name) {
            // 复制一个节点（避免共享同一地址）
            Node* newNode = new Node(*current);
            double rating = ui->ratingSlider->value() / 10.0;
            newNode->setIsFavorite(true);
            newNode->setRating(rating);
            newNode->setNext(NULL);//断开 next 指针，防止复制整个链表
            favoriteList.addNode(newNode);
            favoriteList.sortListByRating();
            favoriteList.displayList();

            found = true;
            return;
        }
        current = current->getNext();
    }

    // 根据是否找到景点，显示相应的消息框
    if (found) {
        QMessageBox::information(this, "添加成功", "景点已成功添加到收藏夹！");
    } else {
        QMessageBox::warning(this, "添加失败", "未找到该景点！");
    }

    qDebug() << "未找到对应的景点：" << name;
}

void FavoriteWidget::handleDeleteAction(){
    //找到子窗口addWidget的placeInput
    QLineEdit* nameLineEdit = ui->deleteWidget->findChild<QLineEdit*>("placeInput_2");
    if (nameLineEdit) {
        QString name = nameLineEdit->text();  // 获取 QLineEdit 中的文本
        deleteFavoriteByName(name);  // 查找并添加景点
        updateFavoriteDisplay();
        nameLineEdit->clear();
        ui->deleteWidget->hide();   // 隐藏 addWidget
    } else {
        qDebug() << "placeInput_2 控件未找到！";
    }
}

void FavoriteWidget::deleteFavoriteByName(const QString &name)
{
    Node* current = favoriteList.getHead();
    bool found = false;

    while (current) {
        if (current->getName() == name) {
            favoriteList.deleteNodeByName(name);
            favoriteList.sortListByRating();
            qDebug() << "删除成功，更新后的收藏夹：";
            updateFavoriteDisplay();

            found = true;
            break;
        }
        current = current->getNext();
    }

    if (found) {
        QMessageBox::information(this, "删除成功", "景点已成功从收藏夹删除！");
    } else {
        QMessageBox::warning(this, "删除失败", "未找到该景点！");
    }

    qDebug() << (found ? "删除成功：" : "未找到对应的景点：") << name;
}

//同步displayList到listWidget
void FavoriteWidget::updateFavoriteDisplay(){
    ui->listWidget->clear();
    for (int i = 0; i < ui->listWidget->count(); ++i)
        qDebug() << ui->listWidget->item(i)->text();
    // 设置图标大小为 40x40
    ui->listWidget->setIconSize(QSize(40, 40));

    Node* current = favoriteList.getHead();
    if (!current) {
        qDebug() << "没有找到任何收藏的景点！";  // 检查是否有数据
        // 如果链表为空，显示提示信息
        QListWidgetItem* emptyItem = new QListWidgetItem("收藏夹为空");
        emptyItem->setTextAlignment(Qt::AlignHCenter); // 水平居中
        emptyItem->setFlags(Qt::NoItemFlags); // 禁止选中或点击
        ui->listWidget->addItem(emptyItem);
        return;
    }
    while(current){
        //显示的文本
        QString displayText = QString("地点：%1 类型：%2 喜爱程度：%3")
                                  .arg(current->getName())
                                  .arg(current->getType())
                                  .arg(QString::number(current->getRating(),'f',1));//将double数字转为字符串插入到Qstring 保留1位小数
        //创建item并设置文本
        QListWidgetItem* item = new QListWidgetItem(displayText);

        //set icon
        if(current->getType()==BUILDING){
            item->setIcon(QIcon(":/Pamela/image/building.png"));
        }else if(current->getType()==TOLIET){
            item->setIcon(QIcon(":/Pamela/image/toilet.png"));
        }else //current->getType()==RESTERANT)
        {
                item->setIcon(QIcon(":/Pamela/image/restaurant.png"));
        }
        //加item
        ui->listWidget->addItem(item);
        current = current->getNext();
    }
    //for debug确认删除正确更新
    for (int i = 0; i < ui->listWidget->count(); ++i)
        qDebug() << ui->listWidget->item(i)->text();

}

//处理显示窗口
void FavoriteWidget::showAddWidget(){
    ui->addWidget->show();
}
void FavoriteWidget::showDeleteWidget(){
    ui->deleteWidget->show();
}
//-------处理隐藏窗口------
void FavoriteWidget::handleExitAction(){
    saveFavoriteList();  // 保存收藏夹
    this->hide();//ui->FavoriteWidget 是你自己定义的窗口类 FavoriteWidget 本身，它并不是 Qt Designer 里的某个控件
}
void FavoriteWidget::handleExit2Action(){
    ui->addWidget->hide();
}
void FavoriteWidget::handleExit2_2Action(){
    ui->deleteWidget->hide();
}
void FavoriteWidget::saveFavoriteList() {
    QSettings settings("MyApp", "TourGuide");
    settings.beginGroup("Favorites");

    settings.remove("");  // 清除整个 "Favorites" 分组下的所有旧数据
    Node* current = favoriteList.getHead();
    int index = 0;

    while (current) {
        settings.setValue("favorite" + QString::number(index) + "/name", current->getName());
        settings.setValue("favorite" + QString::number(index) + "/rating", current->getRating());
        // 这里可以添加更多的字段，例如类型等
        current = current->getNext();
        index++;
    }

    settings.endGroup();
    qDebug() << "收藏夹已保存";
}

void FavoriteWidget::loadFavoriteList() {
    QSettings settings("MyApp", "TourGuide");
    settings.beginGroup("Favorites");

    int index = 0;
    while (settings.contains("favorite" + QString::number(index) + "/name")) {
        QString name = settings.value("favorite" + QString::number(index) + "/name").toString();
        double rating = settings.value("favorite" + QString::number(index) + "/rating").toDouble();

        // 调用 addFavoriteByName 添加景点到收藏夹
        addFavoriteByName(name);

        // 获取最新的节点并更新其评分
        Node* newNode = favoriteList.getHead();
        while (newNode) {
            if (newNode->getName() == name) {
                newNode->setRating(rating); // 设置读取到的评分
                break;
            }
            newNode = newNode->getNext();
        }

        index++;
    }

    settings.endGroup();

    // 加载完成后，更新显示
    updateFavoriteDisplay();
    qDebug() << "收藏夹已加载";
}


