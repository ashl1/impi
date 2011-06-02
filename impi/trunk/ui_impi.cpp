/*
 * ui_impi.cpp
 *
 *  Created on: Apr 18, 2011
 *      Author: ashl1
 */

#include <QList>
#include <QDebug>
#include "ui_impi.h"
#include "plugins.h"
#include "impi.h"

MenuPluginElement::MenuPluginElement() {
	submenu_ = 0;
	menu_element_for_conf_path_ = 0;
	menu_element_for_file_path_ = 0;
}

MenuPluginElement::MenuPluginElement(const QString& plugin_name, bool can_init_from_conf_path,
		bool can_init_from_file, QMenuBar* parent) {
	submenu_ = new QMenu(parent);
	submenu_->setObjectName(plugin_name);

	this->menu_element_for_conf_path_ = 0;
	this->menu_element_for_file_path_ = 0;

	if (can_init_from_conf_path){
		this->menu_element_for_conf_path_ = new QAction(submenu_);
		submenu_->addAction(this->menu_element_for_conf_path_);
	}
	if (can_init_from_file){
		this->menu_element_for_file_path_ = new QAction(submenu_);
		submenu_->addAction(this->menu_element_for_file_path_);
	}
}

QAction* MenuPluginElement::menu_element_for_conf_path() const {
	return menu_element_for_conf_path_;
}

QAction* MenuPluginElement::menu_element_for_file_path() const {
	return menu_element_for_file_path_;
}

QMenu* MenuPluginElement::submenu() const {
	return submenu_;
}

///////////////////////////////////////////////////////////

void ImpiGUI::SetupUi(QMainWindow *impi_class) {
	main_window_ = impi_class;

	if (main_window_->objectName().isEmpty())
		main_window_->setObjectName(QString::fromUtf8("impiClass"));
	main_window_->resize(800, 600);

	// menu bar
	menubar_ = new QMenuBar(main_window_);
	menubar_->setObjectName(QString::fromUtf8("menubar"));
	menubar_->setGeometry(QRect(0, 0, 800, 25));

	load_menu_ = new QMenu(menubar_);
	load_menu_->setObjectName("load");

	main_window_->setMenuBar(menubar_);
	menubar_->addAction(load_menu_->menuAction());

	// statusbar
	statusbar_ = new QStatusBar(main_window_);
	statusbar_->setObjectName(QString::fromUtf8("statusbar"));
	main_window_->setStatusBar(statusbar_);

	central_widget_ = new QWidget(main_window_);
	central_widget_->setObjectName(QString::fromUtf8("centralwidget"));
	main_window_->setCentralWidget(central_widget_);

	RetranslateUi();

	// connect slots
	QMetaObject::connectSlotsByName(main_window_);
} // setupUi

void ImpiGUI::RetranslateUi(){
	main_window_->setWindowTitle("Impi");

	load_menu_->setTitle(QObject::tr("Load"));
} // retranslateUi

void ImpiGUI::PluginChanged(){
	const Plugin* sender_plugin = qobject_cast<Plugin* >(sender());
	if (menu_plugins_.find(sender_plugin) != menu_plugins_.end()){
		load_menu_->removeAction(menu_plugins_.find(sender_plugin)->submenu()->menuAction());
	}

	menu_plugins_[sender_plugin] = MenuPluginElement(sender_plugin->fullname(), sender_plugin->can_init_from_conf_path(),
			sender_plugin->can_init_from_file(), menubar_);
	MenuPluginElement* menu_plugin_element = &menu_plugins_[sender_plugin];

	// connect "load" submenu
	// TODO(ashl1future): Do ImportOnline, Import, OpenFile
	if (menu_plugin_element->menu_element_for_conf_path())
		connect(menu_plugin_element->menu_element_for_conf_path(), SIGNAL(triggered()), sender_plugin, SLOT(ImportOnline()));
	if (menu_plugin_element->menu_element_for_file_path())
		connect(menu_plugin_element->menu_element_for_file_path(), SIGNAL(triggered()), main_window_, SLOT(onWantFilePath()));

	// set titles
	QString title;
	title = menu_plugin_element->submenu()->objectName();
	menu_plugin_element->submenu()->setTitle(title);
	if (menu_plugin_element->menu_element_for_conf_path())
		menu_plugin_element->menu_element_for_conf_path()->setText("Choose folder");
	if (menu_plugin_element->menu_element_for_file_path())
		menu_plugin_element->menu_element_for_file_path()->setText("Choose file");

	// search for place to insert
	QMenu* menu_before = NULL;
	QString max_title;
	// find max title first
	foreach(MenuPluginElement elem, menu_plugins_.values()) {
		if (elem.submenu()->objectName() > max_title)
			max_title = elem.submenu()->objectName();
	}
	// find beforeAction
	foreach(MenuPluginElement elem, menu_plugins_.values()) {
		if ((title < elem.submenu()->objectName()) && (elem.submenu()->objectName() < max_title)) {
			menu_before = elem.submenu();
			max_title = elem.submenu()->objectName();
		}
	}
	if (menu_before)
		load_menu_->insertAction(menu_before->menuAction(), menu_plugin_element->submenu()->menuAction());
	else
		load_menu_->addAction(menu_plugin_element->submenu()->menuAction());
}
