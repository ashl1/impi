

#include "impi.h"
#include "dialogs/chooseAccount.h"

Impi::Impi(QWidget *parent)
    : QMainWindow(parent)
{
	plugins = new Plugins(this);
	ui.setupUi(this, plugins);
}

Impi::~Impi()
{

}

void Impi::onWantConfPath(){
	QString dirName = QFileDialog::getExistingDirectory(0, "", QDir::toNativeSeparators(QDir::homePath()+"/.Skype"),
			QFileDialog::ShowDirsOnly);
	if (dirName != ""){
		// parent because sender is a QActino which may be "conf path" or "file",
		//    but only parent QMenu has object name
		PluginInterface* plugin = plugins->Plugin(QObject::sender()->parent()->objectName());

		// check for accounts
	    QVector<QString> accNames;
		QVector<QDir> accPathes;

		plugin->GetClientAccounts(dirName, accPathes, accNames);
		if (accNames.count() == 1)
			plugin->InitFromConfPath(*accPathes.constBegin());
		else{
			ChooseAccountDialog* dialog = new ChooseAccountDialog(accNames);
			if (dialog->exec() == QDialog::Accepted){
				QVector<QDir>::const_iterator pIt = accPathes.constBegin();
				for (QVector<QString>::const_iterator it = accNames.constBegin(); it != accNames.constEnd(); ++it){
					if (dialog->Name() == *it)
						break;
					else
						++pIt;
				}
				plugin->InitFromConfPath(*pIt);
			}else
				return;
			delete dialog;
		}
	}

}
void Impi::onWantFilePath(){
	;
}
