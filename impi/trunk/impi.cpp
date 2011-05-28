#include "impi.h"
#include "dialogs/chooseAccount.h"

void ImpiClass::Init() {
	ui = new ImpiGUI(this);
	ui->SetupUi(this);

	plugins = new Plugins(ui, this);
	plugins->InitializeAll();
}

void ImpiClass::onWantConfPath(){
	// 
	QString dir_path = QFileDialog::getExistingDirectory(0, "", QDir::toNativeSeparators(QDir::homePath()+"/.Skype"),
			QFileDialog::ShowDirsOnly);
	if (dir_path != ""){
		QDir dir;
		// TODO(ashl1future): check for error
		dir.cd(dir_path);

		// parent because sender is a QAction which may be "conf path" or "file",
		//    but only parent QMenu has object name
		plugins->GetPluginByName(sender()->parent()->objectName())->GetClientAccounts(dir);

		// check for accounts
	    QVector<QString> accNames;
		QVector<QDir> accPathes;

//		plugin->GetUserAccounts(dirName, accPathes, accNames);
//		if (accNames.count() == 1)
//			plugin->InitFromConfPath(*accPathes.constBegin());
//		else{
//			ChooseAccountDialog* dialog = new ChooseAccountDialog(accNames);
//			if (dialog->exec() == QDialog::Accepted){
//				QVector<QDir>::const_iterator pIt = accPathes.constBegin();
//				for (QVector<QString>::const_iterator it = accNames.constBegin(); it != accNames.constEnd(); ++it){
//					if (dialog->Name() == *it)
//						break;
//					else
//						++pIt;
//				}
//				plugin->InitFromConfPath(*pIt);
//			}else
//				return;
//			delete dialog;
//		}
	}

}
void ImpiClass::onWantFilePath(){
	;
}
