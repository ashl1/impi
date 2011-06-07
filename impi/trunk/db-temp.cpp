/*
 * db-temp.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: ashl1
 */

#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
// TODO(ashl1future):
//#include <QSQLiteDriver>
#include <QVariant>

#include "db-temp.h"
#include "plugins.h"

bool DBTemp::importOrDie(const QList<PluginMessage*> messages) {
	db = QSqlDatabase::addDatabase("QSQLITE");
	QString db_absolute_path = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("db");
	db.setDatabaseName(QDir::toNativeSeparators(db_absolute_path));
	qDebug() << (db.open()? "Db opened": "Error while opening db");
	if (IsValidDB()) {
		qDebug() << "valid DB";
	} else {
		qDebug() << "invalid DB";
		MakeDB();
	}

	QSqlQuery query;
	query.setForwardOnly(true);

	// create if needed


	QMap<const PluginChat*, quint64> chats_in_db;
	QMap<const void*, quint64> used;

	// TODO(ashl1future): check QSQLiteDriver for feature QSqlDriver::Transactions
	// TODO(ashl1future): make queries in transaction

	quint64 i = 0;
	QDateTime time_started = QDateTime::currentDateTime();

	for (QList<PluginMessage*>::const_iterator messages_iterator = messages.constBegin();
			messages_iterator != messages.end(); ++messages_iterator) {
		const PluginAccount* account = (*messages_iterator)->account_;

		// account
		quint64 account_id;
		if (used.find((const void*)account) != used.end()) {
			account_id = used[(const void*)account];
		} else {
			// protocol
			quint64 protocol_id;
			if (used.find((const void*)account->protocol_) != used.end()) {
				protocol_id = used[(const void*)account->protocol_];
			} else {
				query.prepare("SELECT id FROM protocols WHERE name = :name");
				query.bindValue(":name", account->protocol_->name_);
				query.exec();
				if (query.next()) {
					protocol_id = query.value(0).toULongLong();
				} else {
					query.prepare("INSERT INTO Protocols(name) VALUES(:name)");
					query.bindValue(":name", account->protocol_->name_);
					query.exec();
					protocol_id = query.lastInsertId().toULongLong();
					used[(const void*)account->protocol_] = protocol_id;
				}
			}
	//		qDebug() << "prot" << protocol_id;

			// user
			quint64 user_id;
			if (used.find((const void*)account->user_) != used.end()) {
				user_id = used[(const void*)account->user_];
			} else {
				query.prepare("SELECT id FROM Users WHERE firstname = :name");
				query.bindValue(":name", account->user_->firstname_);
				query.exec();
				if (query.next()) {
					user_id = query.value(0).toULongLong();
				} else {
					query.prepare("INSERT INTO Users(firstname) VALUES(:name)");
					query.bindValue(":name", account->user_->firstname_);
					query.exec();
					user_id = query.lastInsertId().toULongLong();
					used[(const void*)account->user_] = user_id;
				}
			}
//			qDebug() << "user" << user_id;

			query.prepare("SELECT id FROM Accounts WHERE protocolId = :protocolId and userId = :userId");
			query.bindValue(":protocolId", protocol_id);
			query.bindValue(":userId", user_id);
			query.exec();
			if (query.next()) {
				account_id = query.value(0).toULongLong();
			} else {
				query.prepare("INSERT INTO Accounts(protocolID, userID) VALUES(:protocolID, :userID)");
				query.bindValue(":protocolID", protocol_id);
				query.bindValue(":userID", user_id);
				query.exec();
				account_id = query.lastInsertId().toULongLong();
				used[(const void*)account] = account_id;
			}
		}
//		qDebug() << "account" << account_id;

		// client
		quint64 client_id;
		if (used.find((const void*)(*messages_iterator)->client_) != used.end()) {
			client_id = used[(*messages_iterator)->client_];
		} else {
			query.prepare("SELECT id FROM Clients WHERE name = :name");
			query.bindValue(":name", (*messages_iterator)->client_->name_);
			query.exec();
			if (query.next()) {
				client_id = query.value(0).toULongLong();
			} else {
				query.prepare("INSERT INTO Clients(name) VALUES(:name)");
				query.bindValue(":name", (*messages_iterator)->client_->name_);
				query.exec();
				client_id = query.lastInsertId().toULongLong();
				used[(*messages_iterator)->client_] = client_id;
			}
		}
//		qDebug() << "client" << client_id;

		// chat
		quint64 chat_id;
		if (chats_in_db.constFind((*messages_iterator)->chat_) == chats_in_db.constEnd()) {
			query.prepare("INSERT INTO Chats(timeStart) VALUES(:timeStart)");
			query.bindValue(":timeStarted", (*messages_iterator)->time_);
			query.exec();
			chat_id = query.lastInsertId().toULongLong();
			chats_in_db[(*messages_iterator)->chat_] = chat_id;
		} else {
			chat_id = chats_in_db[(*messages_iterator)->chat_];
		}
//		qDebug() << "chat" << chat_id;

		// message
		query.prepare("INSERT INTO Messages(accountID, body, chatID, clientID, time)"
				"VALUES(:accountID, :body, :chatID, :clientID, :time)");
		query.bindValue(":accountID", account_id);
		query.bindValue(":body", (*messages_iterator)->body_);
		query.bindValue(":chatID", chat_id);
		query.bindValue(":clientID", client_id);
		query.bindValue(":time", (*messages_iterator)->time_);
		query.exec();
//		qDebug() << "mess" << query.lastInsertId().toULongLong();

		++i;
		qDebug() << i << "/" << messages.count() << "finished";
		QCoreApplication::processEvents();
	}
	quint64 seconds_elapsed = time_started.secsTo(QDateTime::currentDateTime());
	quint64 minutes_elapsed = seconds_elapsed / 60;
	seconds_elapsed = seconds_elapsed % 60;
	qDebug() << minutes_elapsed << "min" << seconds_elapsed << "sec importing to DB";

	db.close();
	return true;
}

bool DBTemp::IsValidDB() const {
	QSqlQuery query;
	query.setForwardOnly(true);
	if (!query.exec("SELECT count(*) FROM sqlite_master WHERE type='table' and"
			"(name = 'accounts' or name = 'chats' or name = 'clients' or name = 'messages' or"
			"name = 'protocols' or name = 'users')"))
		return false;
	if (!query.next())
		return false;
	if (query.value(0).toInt() == 6)
		return true;
	else
		return false;
}

void DBTemp::MakeDB() const {
	QSqlQuery query;
	query.exec("CREATE TABLE 'CHATS' ("
			"'id' INTEGER PRIMARY KEY ASC,"
			"'timeLastModified' INTEGER,"
			"'timeStarted' INTEGER"
			");");
	query.exec("CREATE TABLE 'CLIENTS' ("
			"'id' INTEGER PRIMARY KEY ASC,"
			"'name' TEXT UNIQUE NOT NULL"
			");");
	query.exec("CREATE TABLE 'PROTOCOLS' ("
			"'id' INTEGER PRIMARY KEY ASC,"
			"'name' TEXT UNIQUE NOT NULL"
			");");
	query.exec("CREATE TABLE 'USERS' ("
			"'id' INTEGER PRIMARY KEY ASC,"
			"'firstname' TEXT,"
			"'middlename' TEXT,"
			"'soname' TEXT"
			");");
	query.exec("CREATE TABLE 'ACCOUNTS' ("
			"'id' INTEGER PRIMARY KEY ASC,"
			"'protocolID' INTEGER NOT NULL REFERENCES PROTOCOLS(id) ON DELETE RESTRICT ON UPDATE CASCADE,"
			"'userID' INTEGER NOT NULL REFERENCES USERS(id) ON DELETE RESTRICT ON UPDATE CASCADE"
			");");
	query.exec("CREATE TABLE 'MESSAGES' ("
			"'id' INTEGER PRIMARY KEY,"
			"'accountID' INTEGER NOT NULL REFERENCES ACCOUNTS(id)  ON DELETE RESTRICT ON UPDATE CASCADE,"
			"'body' TEXT,"
			"'chatID' INTEGER NOT NULL REFERENCES CHATS(id)  ON DELETE RESTRICT ON UPDATE CASCADE,"
			"'clientID' INTEGER NOT NULL REFERENCES CLIENTS(id)  ON DELETE RESTRICT ON UPDATE CASCADE,"
			"'time' INTEGER"
			");");
}
