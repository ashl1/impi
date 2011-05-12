/*
 * plugins.h
 *
 *  Created on: 23.04.2011
 *      Author: ashl1
 */

#ifndef PLUGINS_H_
#define PLUGINS_H_

#include <QObject>
#include <QVector>
#include <QPluginLoader>
#include "plugin-interface.h"

class PluginDummy: public QObject{
	Q_OBJECT

public:
	PluginDummy(QObject* parent = 0):QObject(parent){};

signals:
	void Initialized();
public slots:
	void SlotInitialized();
};

class Plugin: public QObject{
	Q_OBJECT

private:
	QPluginLoader* pluginLoader;
	PluginInterface* plugin;
	PluginDummy* pluginDummy;

public:
	Plugin(QObject* parent = 0):QObject(parent){};
	~Plugin();

	PluginInterface* GetPlugin() const;
	void Load(const QString& libname);
	void Unload();

public slots:
	void Initialized();
};

class Plugins: public QObject{
	Q_OBJECT
private:
/* should use Q..List instead
 *
 */
	QVector<Plugin*> plugins;

public:
	Plugins(QObject* parent/*= 0*/):QObject(parent){};
	~Plugins();

/* should rewrite to scan all plugins and check it versions
 *
 */
	void InitializeAll();

	quint8 Count();

	// load plugin from standard plugin path (%app%/plugins)
	PluginInterface* LoadPlugin(const QString& libname);
	void UnloadPlugin(PluginInterface* plugin);

	// get PluginInterface* by it's (QObject's) name
	PluginInterface* GetPlugin(QString fullname) const;
	PluginInterface* GetPlugin(quint8 position) const;
};

#endif /* PLUGINS_H_ */
