#ifndef IMPI_H
#define IMPI_H

#include <QtGui/QMainWindow>
#include <QVector>
#include "ui_impi.h"
#include "plugin-interface.h"

class Impi : public QMainWindow
{
    Q_OBJECT

private:
    ImpiClass ui;

public:
    QVector<PluginInterface*> plugins;

public:
    Impi(QWidget *parent = 0);
    ~Impi();

private:
    void PluginsInit();

public slots:
	void onWantConfPath();
	void onWantFilePath();
};

#endif // IMPI_H
