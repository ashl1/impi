#ifndef IMPI_H
#define IMPI_H

#include <QtGui/QMainWindow>

#include <QFileDialog>
#include <QMessageBox>

#include "ui_impi.h"
#include "plugin-interface.h"
#include "plugins.h"

namespace Impi{

class Impi : public QMainWindow
{
    Q_OBJECT

private:
    ImpiClass ui;
    Plugins* plugins;
public:
    Impi(QWidget *parent = 0);
    ~Impi();

public slots:
	void onWantConfPath();
	void onWantFilePath();
};
} // namespace Impi

#endif // IMPI_H
