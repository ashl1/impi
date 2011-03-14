#ifndef IMPI_H
#define IMPI_H

#include <QtGui/QMainWindow>
#include "ui_impi.h"

class Impi : public QMainWindow
{
    Q_OBJECT

public:
    Impi(QWidget *parent = 0);
    ~Impi();

private:
    Ui::ImpiClass ui;
};

#endif // IMPI_H
