/*
 * skype-linux.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#include <qplugin.h>
#include "skype-linux.h"

using ::SkypeLinux::PluginSkypeLinux;
using ::SkypeLinux::AccountSkypeLinux;
using ::SkypeLinux::ChatSkypeLinux;
using ::SkypeLinux::MessageSkypeLinux;
using ::SkypeLinux::ProtocolSkypeLinux;
using ::SkypeLinux::UserSkypeLinux;

/* virtual */ quint32 AccountSkypeLinux::GetNumber() const{
	;
}

/* virtual */ Impi::PluginProtocol* AccountSkypeLinux::GetProtocol() const{
	;
}

/* virtual */ Impi::PluginUser* AccountSkypeLinux::GetUser() const{
	;
}

///////////////////////////////////////////////////////////

/* virtual */ QDateTime ChatSkypeLinux::GetStartTime() const{
	;
}

/* virtual */ QDateTime ChatSkypeLinux::GetModifiedTime() const{
	;
}

///////////////////////////////////////////////////////////

/* virtual */ QDateTime MessageSkypeLinux::GetTime() const{
	;
}

/* virtual */ QString MessageSkypeLinux::GetBody() const{
	;
}

/* virtual */ Impi::PluginAccount* MessageSkypeLinux::GetAccount() const{
	;
}

/* virtual */ Impi::PluginClient* MessageSkypeLinux::GetClient() const{
	;
}

///////////////////////////////////////////////////////////

/* virtual */ QString ProtocolSkypeLinux::GetName() const{
	;
}

/* virtual */ quint32 ProtocolSkypeLinux::GetNumber() const{
	;
}

///////////////////////////////////////////////////////////

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

/*virtual */ void PluginSkypeLinux::InitializeEvents(QObject* pluginDummy){
	QObject::connect(this, SIGNAL(Initialized()), pluginDummy, SLOT(SlotInitialized()));
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
	emit Initialized();
}

/* virtual */ void PluginSkypeLinux::InitFromFile(const QDir& path, const QString& filename){
	//throw Error(1);
}

/* virtual */ const Impi::PluginClient* PluginSkypeLinux::GetClient() const{
	;
}

/* virtual */ void PluginSkypeLinux::InitProtocols(){
	ProtocolSkypeLinux* protocol = new ProtocolSkypeLinux("Skype", 0);
	protocols.append(protocol);
}
/* virtual */ const QVector<Impi::PluginProtocol*>* PluginSkypeLinux::GetProtocols() const{
	return &protocols;
}

/* virtual */ void PluginSkypeLinux::InitUsers(){
	;
}
/* virtual */ const QVector<Impi::PluginUser*>* PluginSkypeLinux::GetUsers() const{
	;
}

/* virtual */ void PluginSkypeLinux::InitAccounts(){
	;
}
/* virtual */ const QVector<Impi::PluginAccount*>* PluginSkypeLinux::GetAccounts() const{
	;
}

/* virtual */ void PluginSkypeLinux::InitNextChats(quint16 limit, QDateTime& time){
	;
}
/* virtual */ const QVector<Impi::PluginChat*>* PluginSkypeLinux::GetChats() const{
	;
}

/* virtual */ void PluginSkypeLinux::InitNextMessages(quint16 limit, QDateTime& time){
	;
}
/* virtual */ const QVector<Impi::PluginMessage*>* PluginSkypeLinux::GetMessages() const{
	;
}

Q_EXPORT_PLUGIN2(impi-skype-linux, PluginSkypeLinux)
