#include <QtGui>
#include <QApplication>
#include <QPluginLoader>
#include <QObject>

#include "impi.h"
#include "plugins.h"

int main(int argc, char *argv[])
{
	try{
	QApplication a(argc, argv);
    ImpiClass w;
    w.Init();

    w.show();

    return a.exec();
	}
	catch (Error err){
		qCritical() << "Error type: " << err.type << " Error: " << err.text;
		return err.type;
	}
	catch(...){
		qFatal("Unexpected error. The application will be closed now.");
		return 1;
	}
}
