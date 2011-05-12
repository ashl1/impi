#include <QtGui>
#include <QApplication>
#include <QPluginLoader>
#include <QObject>

#include "impi.h"

int main(int argc, char *argv[])
{
	try{
	QApplication a(argc, argv);
    Impi::Impi w;

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
	catch (Impi::Error err){
		qCritical() << "Error type: " << err.type << " Error: " << err.text;
		return err.type;
	}
	catch(...){
		qFatal("Unexpected error. The application will be closed now.");
		return 1;
	}
}
