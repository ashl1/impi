#include <QtGui>
#include <QApplication>
#include <QPluginLoader>
#include <QObject>

#include "impi.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Impi w;

    w.show();

    	/*
    	QString dirName = QFileDialog::getExistingDirectory(0, "", QDir::toNativeSeparators(QDir::homePath()+"/.Skype"), QFileDialog::ShowDirsOnly);
    	if (dirName != "")
    	   	QMessageBox::information(0, "TitleTest003", dirName);

    	QMessageBox::information(0, "TitleTest002", "value: "+
    			QString(interface1->CanInitFromFile()?"true":"false"));
    	*/

    return a.exec();
}
