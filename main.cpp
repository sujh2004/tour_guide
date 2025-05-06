#include "mainwindow.h"
#include "favoritewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    // QApplication a(argc, argv);
    // FavoriteWidget w;
    // w.show();
    // return a.exec();
}
