#ifndef IMPI_H
#define IMPI_H

#include <QtGui/QMainWindow>

#include <QFileDialog>
#include <QMessageBox>

#include "ui_impi.h"
#include "plugin-interface.h"
#include "plugins.h"

class ImpiClass : public QMainWindow
{
    Q_OBJECT

private:
    ImpiGUI* ui;
    Plugins* plugins;
public:
    ImpiClass(QWidget *parent = 0): QMainWindow(parent){};
    ~ImpiClass(){};

    void Init();

    void GotAccounts();

public slots:
	void onWantConfPath();
	void onWantFilePath();
};

#endif // IMPI_H
