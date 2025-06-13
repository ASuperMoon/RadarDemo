#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/favicon.ico")); // 软件图标
    MainWindow w;
    w.show();
    return app.exec();
}
