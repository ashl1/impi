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

/* Plugin's name (major name and minor name). It is shown in the menu and used as object name.
 * major: Skype
 * minor: Linux
 * name at whole (major + " " + minor): Skype Linux
 *
 *
 * This class is not thread-safe
 *
 */

class PluginInterface{
	public:
	virtual ~PluginInterface(){};

	virtual bool can_init_from_file() const = 0;
	virtual QString full_name() const = 0;
	virtual QString GetHomePath() const = 0;
	// path - path to initial dir with may be accounts on it
	// pathes - list of absolute pathes to accounts dir
	// names - list of shown account names
	// must have at least 1 account dir path and name
	virtual void GetUserAccounts(const QDir& path, QVector<QDir>& pathes, QVector<QString>& names) const = 0;
	virtual bool GetNextMessagesOrDie(const QDateTime& time_from, quint64 max_messages_size, QDateTime& last_message_time,
	    		QList<PluginMessage*>& messages) = 0;
    virtual QString major_name() const = 0;
	virtual QString minor_name() const = 0;
    // This method clean all PluginObjects pointers data such as PluginMessage, PluginChat etc. After executing this function
    // old pointers will incorrect.
    // Run this method carefully.
    // TODO(ashl1future)
    virtual void ObjectsClean() = 0;
	virtual QString version() const = 0;
	virtual void working_dir(const QString& path) = 0;
	virtual void working_dir(const QDir& dir) = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "org.impi.ClientInterface/0.1")

#endif /* CLIENTINTERFACE_H_ */
