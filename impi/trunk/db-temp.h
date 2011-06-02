/*
 * db-temp.h
 *
 *  Created on: Jun 1, 2011
 *      Author: ashl1
 */

#ifndef DBTEMP_H_
#define DBTEMP_H_

#include <QObject>
#include <QSqlDatabase>

class PluginMessage;

// TODO(ashl1future): fix crash when exiting from GUI due this is in another from main thread:
//   QSqlQuery::prepare: database not open
class DBTemp: public QObject {
	Q_OBJECT
public:
	DBTemp(QObject* parent = NULL):QObject(parent){};
	~DBTemp(){};

	bool importOrDie(const QList<PluginMessage*> messages);

private:
	bool IsValidDB() const;
	void MakeDB() const;

	QSqlDatabase db;
};

#endif /* DBTEMP_H_ */
