#include "widget_simpledriverbackup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget_SimpleDriverBackup w;
    w.show();

    return a.exec();
}
