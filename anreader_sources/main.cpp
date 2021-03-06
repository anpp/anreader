#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);  

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("anreader");
    QCoreApplication::setApplicationName("anreader");
    QCoreApplication::setApplicationVersion("0.9");

    MainWindow *w = MainWindow::instance();
    w->show();
    int result = a.exec();
    delete w;
    return result;
}
