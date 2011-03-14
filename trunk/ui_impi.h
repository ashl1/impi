/********************************************************************************
** Form generated from reading UI file 'impi.ui'
**
** Created: Mon Mar 14 22:31:12 2011
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

class Ui_ImpiClass
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ImpiClass)
    {
        if (ImpiClass->objectName().isEmpty())
            ImpiClass->setObjectName(QString::fromUtf8("ImpiClass"));
        ImpiClass->resize(800, 600);
        centralwidget = new QWidget(ImpiClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        ImpiClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ImpiClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        ImpiClass->setMenuBar(menubar);
        statusbar = new QStatusBar(ImpiClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        ImpiClass->setStatusBar(statusbar);

        retranslateUi(ImpiClass);

        QMetaObject::connectSlotsByName(ImpiClass);
    } // setupUi

    void retranslateUi(QMainWindow *ImpiClass)
    {
        ImpiClass->setWindowTitle(QApplication::translate("ImpiClass", "MainWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImpiClass: public Ui_ImpiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPI_H
