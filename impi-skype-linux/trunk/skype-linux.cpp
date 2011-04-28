/*
 * skype-linux.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#include <qplugin.h>
#include "skype-linux.h"

/*virtual */ void PluginSkypeLinux::SetParent(QObject* parent){
	//setParent(parent);
	connect(this, SIGNAL(Loaded()), parent, SLOT(Loaded()));
}


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

/*virtual */ void PluginSkypeLinux::GetClientAccounts(const QDir& path, QVector<QDir>& pathes,
		QVector<QString>& names) const{
	// check if the path is valid
	// should be files: "shared.lck" and "shared.xml"
	// directories are an accounts
	if (!path.exists("shared.lck") || !path.exists("shared.xml")){
		//throw Error(4, "The path is not valid path with accounts");
	}
	pathes.clear();
	names.clear();
	foreach(QString dirName, path.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name)){
		names.append(dirName);
		QDir dir(path);
		dir.cd(dirName);
		pathes.append(dir);
	}
}

/* virtual */ void PluginSkypeLinux::InitFromConfPath(const QDir& path){
	emit Loaded();
}

/* virtual */ void PluginSkypeLinux::InitFromFile(const QDir& path, const QString& filename){
	//throw Error(1);
}

/* virtual */ QObject* PluginSkypeLinux::Accounts(quint32 count, quint32 from){
	;
}

/* virtual */ QObject* PluginSkypeLinux::Clients(quint32 count, quint32 from){
	;
}

/* virtual */ QObject* PluginSkypeLinux::Messages(quint64 count, quint64 from){
	;
}

/* virtual */ QObject* PluginSkypeLinux::Users(quint32 count, quint32 from){
	;
}

Q_EXPORT_PLUGIN2(impi-skype-linux, PluginSkypeLinux)
