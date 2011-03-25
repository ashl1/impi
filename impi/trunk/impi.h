#ifndef IMPI_H
#define IMPI_H

#include <QtGui/QMainWindow>
#include "ui_impi.h"

class impi : public QMainWindow
{
    Q_OBJECT

public:
    impi(QWidget *parent = 0);
    ~impi();

private:
    Ui::impiClass ui;
};

#endif // IMPI_H
