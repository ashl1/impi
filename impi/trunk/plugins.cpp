/*
 * plugins.cpp
 *
 *  Created on: 23.04.2011
 *      Author: ashl1
 */

#include "plugins.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QtDebug>
#include <QString>
#include <QDir>

void PluginDummy::SlotInitialized(){
	emit Initialized();
}

///////////////////////////////////////////////////////////

Plugin::~Plugin(){
	this->Unload();
}

PluginInterface* Plugin::GetPlugin()const{
	return plugin;
}

void Plugin::Load(const QString& libname){
	QDir dir(QCoreApplication::applicationDirPath());
		if (!dir.cd("plugins"))
			throw Error(1);
	pluginLoader = new QPluginLoader(dir.absoluteFilePath(libname), this);
	QObject* pluginRaw = pluginLoader->instance();
	plugin = qobject_cast<PluginInterface *>(pluginRaw);
	if (!pluginLoader->isLoaded())
		throw Error(6, QString("Plugin: ") + pluginLoader->errorString());

	pluginDummy = new PluginDummy(pluginRaw);
	connect(pluginDummy, SIGNAL(Initialized()), this, SLOT(Initialized()));

	plugin->InitializeEvents(pluginDummy);
}

void Plugin::Unload(){
	if (pluginLoader && pluginLoader->isLoaded()){
		plugin = NULL;
		if(!pluginLoader->unload())
			throw Error(3, QString("Cannot unload plugin: ") + plugin->FullName());
	}
}

void Plugin::Initialized(){
	QMessageBox::information(0, QString("Test"), QString("Slot Loaded Test"));
}

///////////////////////////////////////////////////////////

Plugins::~Plugins(){
	plugins.clear();
}

void Plugins::InitializeAll(){
	// initialize all plugins
	QDir dir(QCoreApplication::applicationDirPath());
	if (!dir.cd("plugins"))
		throw Error(1);
	/* 1) check filename too and throw Error if not find
	 * 2) scan all
	 */
	QString str("libimpi-skype-linux.so");
	LoadPlugin(str);
}

quint8 Plugins::Count(){
	return plugins.count();
}

PluginInterface* Plugins::LoadPlugin(const QString& libname){
	Plugin* plugin = new Plugin(this);
	plugin->Load(libname);
	plugins.append(plugin);
	return plugin->GetPlugin();
}

void Plugins::UnloadPlugin(PluginInterface* plugin){
	QVector<Plugin*>::iterator it = plugins.begin();
	for (; ((*it)->GetPlugin() != plugin) && (it != plugins.end()); ++it)
		;
	if (it != plugins.end())
		(*it)->Unload();
}

PluginInterface* Plugins::GetPlugin(QString fullname) const{
	for (QVector<Plugin*>::const_iterator it = plugins.constBegin(); it != plugins.constEnd(); ++it)
		if ((*it)->GetPlugin()->FullName() == fullname)
			return (*it)->GetPlugin();
	return NULL;
}

PluginInterface* Plugins::GetPlugin(quint8 position) const{
	return plugins.at(position)->GetPlugin();
}
