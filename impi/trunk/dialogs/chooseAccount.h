/*
 * chooseAccount.h
 *
 *  Created on: 24.04.2011
 *      Author: ashl1
 */

#ifndef CHOOSEACCOUNT_H_
#define CHOOSEACCOUNT_H_

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QVector>

class ChooseAccountDialog: public QDialog{
	Q_OBJECT
private:
	QListWidget* list;
public:
	ChooseAccountDialog(QVector<QString>& values, QWidget* parent = 0);

	QString Name() const;
};

#endif /* CHOOSEACCOUNT_H_ */
