#include "routeinputdialog.h"
#include <QLabel>
#include <QFormLayout>
#include <QMessageBox>

RouteInputDialog::RouteInputDialog(QWidget *parent)
    : QDialog(parent)
{
    // 创建输入框
    startPointEdit = new QLineEdit(this);
    endPointEdit = new QLineEdit(this);
    confirmButton = new QPushButton("确定", this);

    // 布局
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("起点:", startPointEdit);
    formLayout->addRow("终点:", endPointEdit);
    formLayout->addWidget(confirmButton);

    setLayout(formLayout);

    // 连接信号和槽
    connect(confirmButton, &QPushButton::clicked, this, &RouteInputDialog::onConfirm);
}

QString RouteInputDialog::getStartPoint() const
{
    return startPointEdit->text();
}

QString RouteInputDialog::getEndPoint() const
{
    return endPointEdit->text();
}

void RouteInputDialog::onConfirm()
{
    if (startPointEdit->text().isEmpty() || endPointEdit->text().isEmpty()) {
        QMessageBox::warning(this, "输入错误", "起点和终点不能为空！");
        return;
    }
    accept();  // 关闭对话框并确认输入
}
