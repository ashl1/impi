/*
 * skype-linux.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef SKYPELINUX_H_
#define SKYPELINUX_H_

#include <QObject>
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
	virtual ~PluginSkypeLinux();

	virtual QString Version() const;
	virtual QString FullName() const;
	virtual QString MajorName() const;
	virtual QString MinorName() const;
	virtual bool CanInitFromFile() const;

	virtual void InitFromConfPath(QDir* path);
	virtual void InitFromFile(QDir* path, QString* filename);

	virtual void Accounts(quint32 count, quint32 from, QObject* accounts);
	virtual void Clients(quint32 count, quint32 from, QObject* clients);
	virtual void Messages(quint64 count, quint64 from, QObject* messages);
	virtual void Users(quint32 count, quint32 from, QObject* users);
};

#endif /* SKYPELINUX_H_ */
