/*
 * objects.h
 *
 *  Created on: Apr 17, 2011
 *      Author: ashl1
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <QtCore>

namespace Impi{

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

	class PluginAccount{
	public:
		// get number in qvector
		virtual quint32 GetNumber() const = 0;

		virtual PluginProtocol* GetProtocol() const = 0;
		virtual PluginUser* GetUser() const = 0;
	};

	class PluginChat{
	public:
		virtual QDateTime GetStartTime() const = 0;
		virtual QDateTime GetModifiedTime() const = 0;
	};

	class PluginClient{
	public:
		virtual QString GetName() const = 0;
	};

	class PluginMessage{
	public:
		virtual QDateTime GetTime() const = 0;
		virtual QString GetBody() const = 0;

		virtual PluginAccount* GetAccount() const = 0;
		virtual PluginClient* GetClient() const = 0;
	};

	class PluginProtocol{
	public:
		virtual QString GetName() const = 0;
		// get number in qvector
		virtual quint32 GetNumber() const = 0;
	};

	class PluginUser{

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
} // namespace Impi

#endif /* OBJECTS_H_ */
