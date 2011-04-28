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
#include "plugin-interface.h"

class QString;

class Plugins: public QObject{
	Q_OBJECT
private:
	QVector<QPluginLoader*> pluginLoaders;
	QVector<PluginInterface*> plugins;

public:
/* should rewrite to scan all plugins and check it versions
 *
 */
	Plugins(QObject* parent = 0);
	~Plugins();

	quint8 Count();

	// load plugin from standard plugin path (%app%/plugins)
	PluginInterface* LoadPlugin(const QString& libname);
	void UnloadPlugin(PluginInterface* plugin);

	PluginInterface* Plugin(QString fullname) const;
	PluginInterface* Plugin(quint8 position) const;

public slots:
	void Loaded();
};

#endif /* PLUGINS_H_ */
