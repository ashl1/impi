/*
 * chooseAccount.cpp
 *
 *  Created on: 24.04.2011
 *      Author: ashl1
 */

#include "chooseAccount.h"
#include <QGridLayout>

using ::Impi::ChooseAccountDialog;

ChooseAccountDialog::ChooseAccountDialog(QVector<QString>& values, QWidget* parent/*= 0*/):
	QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint){

	list = new QListWidget(this);
	list->setSelectionMode(QAbstractItemView::SingleSelection);
	list->setSortingEnabled(true);

	for (QVector<QString>::const_iterator it = values.begin(); it != values.end(); ++it)
		list->addItem(*it);

	QPushButton* butOK = new QPushButton("&Ok", this);
	QPushButton* butCancel = new QPushButton("&Cancel", this);\

	connect(butOK, SIGNAL(clicked()), SLOT(accept()));
	connect(butCancel, SIGNAL(clicked()), SLOT(reject()));

	QGridLayout* layout = new QGridLayout(this);
	layout->addWidget(list, 0, 0, 1, 2);
	layout->addWidget(butOK, 1, 0);
	layout->addWidget(butCancel, 1, 1);
	setLayout(layout);
}

QString ChooseAccountDialog::Name() const{
	return list->selectedItems().first()->text();
}
