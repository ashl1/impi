/*
 * client-interface.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef CLIENTINTERFACE_H_
#define CLIENTINTERFACE_H_

#include "objects.h"
#include "error.h"

class QDir;
class QString;

/* Plugin's name (major name and minor name). It is shown in the menu and used as object name.
 * major: Skype
 * minor: Linux
 * name at whole (major + " " + minor): Skype Linux
 */

class PluginInterface {
public:
	virtual QString Version() const = 0;
	virtual QString FullName() const = 0;
	virtual QString MajorName() const = 0;
	virtual QString MinorName() const = 0;
	virtual bool CanInitFromFile() const = 0;

	virtual ~PluginInterface() {}

	virtual void InitFromConfPath(QDir* path) = 0;
	virtual void InitFromFile(QDir* path, QString* filename) = 0;

	// for each the object exist: Account, Client, etc
	virtual void Accounts(quint32 count, quint32 from, QObject* accounts) = 0;
	virtual void Clients(quint32 count, quint32 from, QObject* clients) = 0;
	virtual void Messages(quint64 count, quint64 from, QObject* messages) = 0;
	virtual void Users(quint32 count, quint32 from, QObject* users) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "org.impi.ClientInterface/0.1")

#endif /* CLIENTINTERFACE_H_ */
