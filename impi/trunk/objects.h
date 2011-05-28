/*
 * objects.hclass Plugin;
 *
 *  Created on: Apr 17, 2011
 *      Author: ashl1
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <QtCore>

class Account;
class Chat;
class Client;
class Message;
class Protocol;
class User;

class PluginAccount;
class PluginChat;
class PluginClient;
class PluginMessage;
class PluginProtocol;
class PluginUser;

///////////////////////////////////////////////////////////

class PluginAccount {
public:
	PluginAccount(const QString& name, const PluginProtocol* protocol, const PluginUser* user):
		name_(name), protocol_(protocol), user_(user) {};

	const QString name_;
	const PluginProtocol* protocol_;
	const PluginUser* user_;
};

class PluginChat {
public:
	PluginChat(const QDateTime& modified_time, const QDateTime& start_time):
		modified_time_(modified_time), start_time_(start_time) {};

	const QDateTime modified_time_;
	const QDateTime start_time_;
};

class PluginClient {
public:
	PluginClient(const QString& name): name_(name) {};

	const QString name_;
};

class PluginMessage {
public:
	PluginMessage(const PluginAccount* account, const QString& body, const PluginChat* chat,
			const PluginClient* client, const QDateTime& time):
		account_(account), body_(body), chat_(chat), client_(client), time_(time) {};

	const PluginAccount* account_;
	const QString body_;
	const PluginChat* chat_;
	const PluginClient* client_;
	const QDateTime time_;
};

class PluginProtocol {
public:
	PluginProtocol(const QString& name): name_(name) {};

	const QString name_;
};

class PluginUser {
public:
	PluginUser(const QString& firstname, const QString& lastname, const QString& middlename):
		firstname_(firstname), lastname_(lastname), middlename_(middlename) {};

	const QString firstname_;
	const QString lastname_;
	const QString middlename_;
};

///////////////////////////////////////////////////////////

class ProgramAccount{

};

class ProgramClient{

};

class ProgramMessage{

};

class ProgramProtocol{

};

class ProgramUser{

};

///////////////////////////////////////////////////////////


class DBObject{};

//class Account: public DBObject {
//private:
//	QString ID;
//
//protected:
//	QLinkedList<Message*> message;
//	const QLinkedList<Account*>::iterator protocol;
//	const QLinkedList<Account*>::iterator user;
//
//public:
//	Account(QObject* parent/* = 0*/);
//	Account(const QString& ID, const User& user, const Protocol& protocol);
//};

class Client: public DBObject {
};

class Message: public DBObject {
	QString body;
//	Account* account;
	QDateTime datetime;
};

class Protocol: public DBObject {
	QString name;
};

class User: public DBObject {
	QString username;
};

class UserAccount: public DBObject{
		QString name;
	};

#endif /* OBJECTS_H_ */
