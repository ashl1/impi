/********************************************************************************
** Form generated from reading UI file 'impi.ui'
**
** Created: Sun Apr 17 23:03:33 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPI_H
#define UI_IMPI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

#include "plugin-interface.h"
#include "plugins.h"

class MenuPluginElement {
private:
	QMenu* main;
	QAction* confPath;
	QAction* filePath;

public:
	MenuPluginElement();
	MenuPluginElement(QString pluginName, QWidget *parent = 0, bool confPath = false, bool filePath = false);
	~MenuPluginElement();

	QAction* GetConfPath() const;
	QAction* GetFilePath()const;
	QMenu* GetMain()const;
};

class ImpiClass
{
public:

	QWidget *centralwidget;
	QStatusBar *statusbar;
	QMenuBar *menubar;

	QVector<MenuPluginElement> plugins;
	QMenu *load;

    void setupUi(QMainWindow *impiClass, Plugins* plugins);

    void retranslateUi(QMainWindow *impiClass);

};

#endif // UI_IMPI_H
