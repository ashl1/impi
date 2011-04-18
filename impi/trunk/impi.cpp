#include "impi.h"
#include <QFileDialog>
#include <QMessageBox>

Impi::Impi(QWidget *parent)
    : QMainWindow(parent)
{
	PluginsInit();
	ui.setupUi(this, plugins);
}

Impi::~Impi()
{

}

void Impi::PluginsInit(){
/* should rewrite to scan all plugins and check it versions
 *
 */
	QDir dir(QApplication::applicationDirPath());
	if (!dir.cd("plugins"))
		throw Error(1);
/* 1) check filename too and throw Error if not find
 * 2) scan all
 */
	QPluginLoader loader(dir.absoluteFilePath(QString("libimpi-skype-linux.so")));
	plugins.append(qobject_cast<PluginInterface *>(loader.instance()));
}

void Impi::onWantConfPath(){
	QString dirName = QFileDialog::getExistingDirectory(0, "", QDir::toNativeSeparators(QDir::homePath()+"/.Skype"),
			QFileDialog::ShowDirsOnly);
	if (dirName != "")
		QMessageBox::information(0, "TitleTest003", dirName);

}
void Impi::onWantFilePath(){
	;
}
