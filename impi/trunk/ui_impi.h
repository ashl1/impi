/********************************************************************************
** Form generated from reading UI file 'impi.ui'
**
** Created: Fri Mar 25 14:31:09 2011
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
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_impiClass
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *impiClass)
    {
        if (impiClass->objectName().isEmpty())
            impiClass->setObjectName(QString::fromUtf8("impiClass"));
        impiClass->resize(800, 600);
        centralwidget = new QWidget(impiClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        impiClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(impiClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        impiClass->setMenuBar(menubar);
        statusbar = new QStatusBar(impiClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        impiClass->setStatusBar(statusbar);

        retranslateUi(impiClass);

        QMetaObject::connectSlotsByName(impiClass);
    } // setupUi

    void retranslateUi(QMainWindow *impiClass)
    {
        impiClass->setWindowTitle(QApplication::translate("impiClass", "MainWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class impiClass: public Ui_impiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPI_H
