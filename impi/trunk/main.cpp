#include <QtGui>
#include <QApplication>
#include <QPluginLoader>
#include <QObject>

#include "impi.h"
#include "client-interface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Impi w;
    w.show();

    QDir dir(QApplication::applicationDirPath());
    if (!dir.cd("plugins")){
    	QMessageBox::critical(0, "TitleTest001", """plugins"" directory does not exist");
    }else{
    	QPluginLoader loader(dir.absoluteFilePath(QString("libimpi-skype-linux.so")));
    	ClientInterface* interface1 = qobject_cast<ClientInterface *>(loader.instance());
    	QMessageBox::information(0, "TitleTest002", "value: "+
    			QString(interface1->CanInitFromFile()?"true":"false"));
    }

    return a.exec();
}
