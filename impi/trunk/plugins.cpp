/*
 * plugins.cpp
 *
 *  Created on: 23.04.2011
 *      Author: ashl1
 */

#include "plugins.h"
#include <QApplication>
#include <QMessageBox>

Plugins::Plugins(QObject* parent/*= 0*/){
	setParent(parent);
	// initialize all plugins
		QDir dir(QApplication::applicationDirPath());
		if (!dir.cd("plugins"))
			throw Error(1);
	/* 1) check filename too and throw Error if not find
	 * 2) scan all
	 */
	QString str("libimpi-skype-linux.so");
	LoadPlugin(str);
}
Plugins::~Plugins(){
	foreach(QPluginLoader* pl, pluginLoaders)
		if(!pl->unload())
			throw Error(3, QString("Cannot unload plugin when ~Plugins"));

	plugins.clear();
	pluginLoaders.clear();
}

quint8 Plugins::Count(){
	return plugins.count();
}

PluginInterface* Plugins::LoadPlugin(const QString& libname){
	QDir dir(QApplication::applicationDirPath());
		if (!dir.cd("plugins"))
			throw Error(1);
	QPluginLoader* pluginLoader = new QPluginLoader(dir.absoluteFilePath(libname), this);
	pluginLoaders.append(pluginLoader);
	PluginInterface* plugin = qobject_cast<PluginInterface *>(pluginLoader->instance());
	// must call this due to connection to this object's (Plugins) slot Loaded
	plugin->SetParent(this);
	plugins.append(plugin);
	return plugin;
}

void Plugins::UnloadPlugin(PluginInterface* plugin){
	QVector<QPluginLoader*>::iterator plIt = pluginLoaders.begin();
	QVector<PluginInterface*>::const_iterator it = plugins.constBegin();
	for (; (*it != plugin) && (it != plugins.constEnd()); ++it)
		++plIt;
	if (!(*plIt)->unload())
		throw Error(3, QString("Cannot unload plugin: ")+(*it)->FullName());
}

PluginInterface* Plugins::Plugin(QString fullname) const{
	for (QVector<PluginInterface*>::const_iterator it = plugins.constBegin(); it != plugins.constEnd(); ++it)
		if ((*it)->FullName() == fullname)
			return *it;
	return 0;
}

PluginInterface* Plugins::Plugin(quint8 position) const{
	return plugins.at(position);
}

void Plugins::Loaded(){
	QMessageBox::information(0, QString("Test"), QString("Test"));
}
