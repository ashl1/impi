#include "impi.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Impi w;
    w.show();
    return a.exec();
}
