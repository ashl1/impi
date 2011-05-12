/*
 * client-interface.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef CLIENTINTERFACE_H_
#define CLIENTINTERFACE_H_

#include "error.h"
#include "objects.h"

namespace Impi{

	//class QDir;
	//class QString;

	/* Plugin's name (major name and minor name). It is shown in the menu and used as object name.
	 * major: Skype
	 * minor: Linux
	 * name at whole (major + " " + minor): Skype Linux
	 */

	class PluginInterface{
	public:
		virtual QString Version() const = 0;
		virtual QString FullName() const = 0;
		virtual QString MajorName() const = 0;
		virtual QString MinorName() const = 0;
		virtual bool CanInitFromFile() const = 0;

		virtual ~PluginInterface(){};

		virtual void InitializeEvents(QObject* pluginDummy) = 0;

		// path - path to initial dir with may be accounts on it
		// pathes - list of absolute pathes to accounts dir
		// names - list of shown account names
		// must have at least 1 account dir path and name
		virtual void GetClientAccounts(const QDir& path, QVector<QDir>& pathes, QVector<QString>& names) const = 0;
		virtual void InitFromConfPath(const QDir& path) = 0;
		virtual void InitFromFile(const QDir& path, const QString& filename) = 0;

		virtual PluginClient* GetClient() const = 0;

		virtual void InitProtocols() = 0;
		virtual QVector<PluginProtocol>* GetProtocols() const = 0;

		virtual void InitUsers() = 0;
		virtual QVector<PluginUser>* GetUsers() const = 0;

		virtual void InitAccounts() = 0;
		virtual QVector<PluginAccount>* GetAccounts() const = 0;

		virtual void InitNextChats(quint16 limit, QDateTime& time) = 0;
		virtual QVector<PluginChat>* GetChats() const = 0;

		virtual void InitNextMessages(quint16 limit, QDateTime& time) = 0;
		virtual QVector<PluginMessage>* GetMessages() const = 0;

	signals:
		virtual void Initialized() = 0;
	};
} // namespace Impi

Q_DECLARE_INTERFACE(Impi::PluginInterface, "org.impi.ClientInterface/0.1")

#endif /* CLIENTINTERFACE_H_ */
