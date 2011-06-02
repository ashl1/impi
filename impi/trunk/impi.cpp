#include "impi.h"
#include "dialogs/chooseAccount.h"

void ImpiClass::Init() {
	ui = new ImpiGUI(this);
	ui->SetupUi(this);

	plugins = new Plugins(ui, this);
	plugins->InitializeAll();
}

void ImpiClass::onWantFilePath(){
	;
}
