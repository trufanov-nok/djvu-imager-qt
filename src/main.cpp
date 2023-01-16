#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":appicon"));
    // use ini format across all platforms
    QSettings::setDefaultFormat(QSettings::IniFormat);
    MainWindow w;
    w.show();
    return a.exec();
}
