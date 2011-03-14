/********************************************************************************
** Form generated from reading UI file 'impi.ui'
**
** Created: Mon Mar 14 19:25:49 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPI_H
#define UI_IMPI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_impiClass
{
public:

    void setupUi(QWidget *impiClass)
    {
        if (impiClass->objectName().isEmpty())
            impiClass->setObjectName(QString::fromUtf8("impiClass"));
        impiClass->resize(400, 300);

        retranslateUi(impiClass);

        QMetaObject::connectSlotsByName(impiClass);
    } // setupUi

    void retranslateUi(QWidget *impiClass)
    {
        impiClass->setWindowTitle(QApplication::translate("impiClass", "impi", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class impiClass: public Ui_impiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPI_H
