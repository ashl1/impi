/*
 * skype-linux.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#include <qplugin.h>
#include "skype-linux.h"

PluginSkypeLinux::~PluginSkypeLinux(){

}

/* virtual */ QString PluginSkypeLinux::Version() const{
	return QString("0.1");
}

/* virtual */ QString PluginSkypeLinux::FullName() const{
	return QString("Skype Linux");
}
/* virtual */ QString PluginSkypeLinux::MajorName() const{
	return QString("Skype");
}
/* virtual */ QString PluginSkypeLinux::MinorName() const{
	return QString("Linux");
}

/* virtual */ bool PluginSkypeLinux::CanInitFromFile() const{
	return false;
}

/* virtual */ void PluginSkypeLinux::InitFromConfPath(QDir* path){

}

/* virtual */ void PluginSkypeLinux::InitFromFile(QDir* path, QString* filename){
	throw Error(1);
}

/* virtual */ void PluginSkypeLinux::Accounts(quint32 count, quint32 from, QObject* accounts){
	;
}

/* virtual */ void PluginSkypeLinux::Clients(quint32 count, quint32 from, QObject* clients){
	;
}

/* virtual */ void PluginSkypeLinux::Messages(quint64 count, quint64 from, QObject* messages){
	;
}

/* virtual */ void PluginSkypeLinux::Users(quint32 count, quint32 from, QObject* users){
	;
}

Q_EXPORT_PLUGIN2(impi-skype-linux, PluginSkypeLinux)
