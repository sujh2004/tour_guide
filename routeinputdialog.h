// routeinputdialog.h
#ifndef ROUTEINPUTDIALOG_H
#define ROUTEINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class RouteInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RouteInputDialog(QWidget *parent = nullptr);
    QString getStartPoint() const;
    QString getEndPoint() const;

private slots:
    void onConfirm();

private:
    QLineEdit *startPointEdit;
    QLineEdit *endPointEdit;
    QPushButton *confirmButton;
};

#endif // ROUTEINPUTDIALOG_H
