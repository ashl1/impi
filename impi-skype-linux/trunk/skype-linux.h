/*
 * skype-linux.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef SKYPELINUX_H_
#define SKYPELINUX_H_

#include <QObject>
#include <QDir>
#include <QStringList>
#include "../impi/plugin-interface.h"

class AccountSkypeLinux : public Account {

};

class ClientSkypeLinux : public Client {

};

class MessageSkypeLinux : public Message {

};

class UserSkypeLinux : public User {

};

class PluginSkypeLinux: public QObject, public PluginInterface {
Q_OBJECT
Q_INTERFACES(PluginInterface)

private:

public:
	virtual void SetParent(QObject* parent);
	virtual ~PluginSkypeLinux();

	virtual QString Version() const;
	virtual QString FullName() const;
	virtual QString MajorName() const;
	virtual QString MinorName() const;
	virtual bool CanInitFromFile() const;

	virtual void GetClientAccounts(const QDir& path, QVector<QDir>& pathes, QVector<QString>& names) const;
	virtual void InitFromConfPath(const QDir& path);
	// should rewrite to use Error class. Do it when make main Impi library
	virtual void InitFromFile(const QDir& path, const QString& filename);

	virtual QObject* Accounts(quint32 count, quint32 from);
	virtual QObject* Clients(quint32 count, quint32 from);
	virtual QObject* Messages(quint64 count, quint64 from);
	virtual QObject* Users(quint32 count, quint32 from);

signals:
	void Loaded();
};

#endif /* SKYPELINUX_H_ */
