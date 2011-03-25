#include "impi.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    impi w;
    w.show();
    return a.exec();
}
