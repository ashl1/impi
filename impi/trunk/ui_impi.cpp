/*
 * ui_impi.cpp
 *
 *  Created on: Apr 18, 2011
 *      Author: ashl1
 */

#include "ui_impi.h"

using ::Impi::MenuPluginElement;
using ::Impi::ImpiClass;

MenuPluginElement::MenuPluginElement(){
	main = 0;
	confPath = 0;
	filePath = 0;
}

MenuPluginElement::MenuPluginElement(QString pluginName, QWidget *parent, bool confPath, bool filePath){
	main = new QMenu(parent);
	main->setObjectName(pluginName);

	this->confPath = 0;
	this->filePath = 0;

	if (confPath){
		this->confPath = new QAction(main);
		main->addAction(this->confPath);
	}
	if (filePath){
		this->filePath = new QAction(main);
		main->addAction(this->filePath);
	}
}

MenuPluginElement::~MenuPluginElement(){
/* is this right? Because QAction have parent impiClass, and when parent destroys, QAction also destroys
*
*/
//		if (confPath)
//			confPath->~QAction();
//		if (filePath)
//			filePath->~QAction();
}

QAction* MenuPluginElement::GetConfPath() const{
	return confPath;
}

QAction* MenuPluginElement::GetFilePath()const{
	return filePath;
}

QMenu* MenuPluginElement::GetMain()const{
	return main;
}

void ImpiClass::setupUi(QMainWindow *impiClass, Plugins* plugins){
	if (impiClass->objectName().isEmpty())
		impiClass->setObjectName(QString::fromUtf8("impiClass"));
	impiClass->resize(800, 600);

	// menu bar
	menubar = new QMenuBar(impiClass);
	menubar->setObjectName(QString::fromUtf8("menubar"));
	menubar->setGeometry(QRect(0, 0, 800, 25));

	load = new QMenu(menubar);
	load->setObjectName("load");

	for(quint8 i = 0; i < plugins->Count(); ++i)
		this->plugins.append(MenuPluginElement(plugins->GetPlugin(i)->FullName(), menubar,
				true, plugins->GetPlugin(i)->CanInitFromFile()));

	impiClass->setMenuBar(menubar);

	menubar->addAction(load->menuAction());

	foreach(MenuPluginElement elem, this->plugins)
		load->addAction(elem.GetMain()->menuAction());

	// statusbar
	statusbar = new QStatusBar(impiClass);
	statusbar->setObjectName(QString::fromUtf8("statusbar"));
	impiClass->setStatusBar(statusbar);

	centralwidget = new QWidget(impiClass);
	centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
	impiClass->setCentralWidget(centralwidget);

	retranslateUi(impiClass);

	// connect slots
	QMetaObject::connectSlotsByName(impiClass);

	// connect from "load" submenu
	foreach(MenuPluginElement elem, this->plugins){
		if (elem.GetConfPath())
			QObject::connect(elem.GetConfPath(), SIGNAL(triggered()), impiClass, SLOT(onWantConfPath()));
		if (elem.GetFilePath())
			QObject::connect(elem.GetFilePath(), SIGNAL(triggered()), impiClass, SLOT(onWantFilePath()));
	}
} // setupUi

void ImpiClass::retranslateUi(QMainWindow *impiClass){
	impiClass->setWindowTitle("Impi");

	load->setTitle(QObject::tr("Load"));

	QString title;
	foreach(MenuPluginElement elem, plugins){
		title = elem.GetMain()->objectName();
		elem.GetMain()->setTitle(title);
		if (elem.GetConfPath())
			elem.GetConfPath()->setText("Choose folder");
		if (elem.GetFilePath())
			elem.GetFilePath()->setText("Choose file");
	}
} // retranslateUi
