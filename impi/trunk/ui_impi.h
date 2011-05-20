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

class Plugin;

// This class represent plugin in GUI menu. It initialize menu entries for plugin (actions like Load, Open etc)
// The submenu object (inside this class) has the objectName - the full name of the plugin needed to determine
// client actions come from.
class MenuPluginElement {
public:
	MenuPluginElement();
	// plugin_name- full name of plugin got from plugin and using in GUI and so on. For addition information
	// see plugin-interface.h
	// parent - parent widget
	// conf_path - plugin could import from one file and open one file to show the user.
	MenuPluginElement(const QString& plugin_name, bool can_init_from_conf_path, bool can_init_from_file, QMenuBar* parent = 0);

	// TODO(ashl1future): why getter is not const? Because text for them set in another. Reprogram this.
	QMenu* submenu() const;
	QAction* menu_element_for_conf_path() const;
	QAction* menu_element_for_file_path() const;

private:
	// The submenu that will be created. Has the objectName - the full name of the plugin needed to determine
	// client actions come from.
	QMenu* submenu_;
	// Menu element for action to import from messenger configuration path
	QAction* menu_element_for_conf_path_;
	// Menu element for action to import file
	QAction* menu_element_for_file_path_;
};

class ImpiGUI: public QObject
{
	Q_OBJECT
public:
	ImpiGUI(QObject* parent = 0): QObject(parent){};
	~ImpiGUI(){};

    void SetupUi(QMainWindow *impi_class);
    void RetranslateUi();

public slots:
	// Should be emitted when Plugin() changed or initialized: full name.
	// Should be emitted only from Plugin class due its dependences
	// Reinitialize it in menu, sort by full name
	void PluginChanged();

private:
	QWidget *central_widget_;
	QStatusBar *statusbar_;
	QMenuBar *menubar_;
	QMenu *load_menu_;
	QMap<const Plugin*, MenuPluginElement> menu_plugins_;

	// Main window pointer. It is used for connect signals such as onWantConfPath and onWantFilePath
	QMainWindow *main_window_;
};

#endif // UI_IMPI_H
