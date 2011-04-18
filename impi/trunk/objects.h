/*
 * objects.h
 *
 *  Created on: Apr 17, 2011
 *      Author: ashl1
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include <QtCore>

class Account;
class Client;
class Message;
class Protocol;
class User;


class Account: public QObject {
	QString ID;
	User* user;
	Protocol* protocol;
};

class Client: public QObject {

};

class Message: public QObject {
	QString body;
	Account* account;
	QDateTime datetime;
};

class Protocol: public QObject {
	QString name;
};

class User: public QObject {
	QString username;
};

#endif /* OBJECTS_H_ */
