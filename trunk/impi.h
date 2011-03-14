#ifndef IMPI_H
#define IMPI_H

#include <QtGui/QWidget>
#include "ui_impi.h"

class Impi : public QWidget
{
    Q_OBJECT

public:
    Impi(QWidget *parent = 0);
    ~Impi();

private:
    Ui::impiClass ui;
};

#endif // IMPI_H
