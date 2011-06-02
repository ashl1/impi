/*
 * plugins.cpp
 *
 *  Created on: 23.04.2011
 *      Author: ashl1
 */

#include "plugins.h"
#include <QCoreApplication>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QString>

#include "plugins.h"

#include "dialogs/chooseAccount.h"
#include "db-temp.h"

///////////////////////////////////////////////////////////

void ImportClass::ChoosedAccount(QString directory_path) {
	qDebug() << QObject::thread();
	qDebug() << "Last importing state";

	QList<PluginMessage*> messages;
	bool all_messages_proceed = false;
	plugin_->working_dir(directory_path);
	// TODO(ashl1future): Choose from DB saved settings. In DB the directory and the machine ID should be specified
	// to determine if the current path the same.
	QDateTime time_from(QDate(2011, 04, 01), QTime(16, 04, 00), Qt::UTC);
	// TODO(ashl1future): Choose from settings or by compile macro in different platforms
	quint64 max_messages_size = 500;

	DBTemp* dbtemp = new DBTemp(this);

	while (!all_messages_proceed) {
		messages.clear();
		qDebug() << "Started from:" << time_from;
		if (!plugin_->GetNextMessagesOrDie(time_from, max_messages_size, time_from,	messages)) {
			qDebug() << "getting unseccessfull: " << plugin_->error();
		}
		qDebug() << "Finished at" << time_from;
		if (messages.count() == 0) {
			all_messages_proceed = true;
		} else {
			dbtemp->importOrDie(messages);
		}
//		for (QList<PluginMessage*>::const_iterator it = messages.constBegin(); it != messages.constEnd(); ++it) {
//			qDebug() << (*it)->time_ << (*it)->body_;
//		}
		QCoreApplication::processEvents();
	}
	qDebug() << "all_messages_proceed";
//	while (true) {
//		// TODO(ashl1future): choose from settings
//		QThread::sleep(1);
//		QCoreApplication::processEvents();
//		qDebug() << "1";
//		emit Finish();
//	}

	qDebug() << "done";
}

void ImportClass::GotConfigurationPath(QString configuration_path) {
	QVector<QDir>* paths = new QVector<QDir>();
	QVector<QString>* names = new QVector<QString>();
	plugin_->GetUserAccounts(QDir(configuration_path), *paths, *names);
	if (paths->count() > 1) {
		emit ChooseAccount(paths, names);
	} else {
		QString path = paths->first().path();
		delete paths;
		delete names;
		ChoosedAccount(path);
	}

}

void ImportClass::Init(QString plugin_fullpath) {
	plugin_loader_ = new QPluginLoader(plugin_fullpath, this);
	plugin_ = qobject_cast<PluginInterface *>(plugin_loader_->instance());
	if (plugin_loader_->isLoaded()) {
		emit Initialized(plugin_->full_name());
	} else {
		;// TODO(ashl1future): handle this throw Error(6, QString("Plugin: ") + plugin_loader_->errorString());
		qDebug() << plugin_loader_->errorString();
	}
}

void ImportClass::Import() {
	emit GetGonfigurationPath(QDir::toNativeSeparators(plugin_->GetHomePath().path()));
}

///////////////////////////////////////////////////////////

void ImportClassThread::run() {
	exec();
	qDebug() << "sleep finished";
}

///////////////////////////////////////////////////////////

ImportManager::~ImportManager() {
	foreach(ImportManager::ImportClassInfo* import_class_info, import_classes_.values())
		DestroyImportClass(import_class_info);
}

void ImportManager::ImportOnline(const Plugin* plugin) {
	if (plugin_import_classes_.find(plugin) == plugin_import_classes_.end()) {
		// There are no ImportClasses for this plugin
		ImportClassInfo* import_class_info = new ImportClassInfo;
		import_class_info->import_class_ = new ImportClass();
		import_class_info->plugin_ = plugin;
		import_class_info->action_ = kImportOnline;
		import_class_info->state_ = kInitializing;
		import_class_info->thread_ = new ImportClassThread();

		import_classes_[import_class_info->import_class_] = import_class_info;
		plugin_import_classes_[plugin].append(import_class_info);
		threads_[import_class_info->thread_] = import_class_info;

		// TODO(ashl1future): connect all needed signals from ImportClass
		connect(import_class_info->import_class_, SIGNAL(Initialized(QString)), this, SLOT(SlotInitialized(QString)));
		connect(import_class_info->import_class_, SIGNAL(GetGonfigurationPath(QString)),
				this, SLOT(GetGonfigurationPath(QString)));
		connect(import_class_info->import_class_, SIGNAL(ChooseAccount(QVector<QDir>*, QVector<QString>*)),
				this, SLOT(ChooseAccount(QVector<QDir>*, QVector<QString>*)));

		connect(import_class_info->thread_, SIGNAL(started()), this, SLOT(ThreadStarted()));

		import_class_info->thread_->start(QThread::LowestPriority);
	} else {
		// There is at least one ImportClass for this plugin
		// TODO(ashl1future): find the ImportClass by action_, check state_
		// If it's kReady, apply actions, else ???
		// If not found, then we should create another ImportClass

		// find for kImportOnline action ImportClass
		QList<ImportClassInfo*>::iterator import_class_info_iterator = plugin_import_classes_[plugin].begin();
		for (; import_class_info_iterator != plugin_import_classes_[plugin].end(); ++import_class_info_iterator) {
			if ((*import_class_info_iterator)->action_ == kImportOnline)
				break;
		}
		if (import_class_info_iterator == plugin_import_classes_[plugin].end()) {
			// TODO(ashl1future):
		} else {
			ImportClassInfo* import_class_info = *import_class_info_iterator;
			if (import_class_info->state_ != kReady) {
				// TODO(ashl1future):
				qDebug() << "point 5";
			} else {
				import_class_info->state_ = kRunning;
				QMetaObject::invokeMethod(import_class_info->import_class_, "Import");
			}
		}
	}
}

//void ImportManager::ImportFile(const Plugin* plugin) {
// TODO(ashl1future)
//}

//void ImportManager::OpenFile(const Plugin* plugin) {
// TODO(ashl1future)
//}

void ImportManager::GetClientAccounts(const Plugin* plugin, const QDir& path){
	// TODO(ashl1future):
}

QPointer<ImportManager> ImportManager::instance_;

ImportManager* ImportManager::instance() {
	if (!instance_)
		instance_ = new ImportManager();
	return instance_;
}

void ImportManager::ChooseAccount(QVector<QDir>* paths, QVector<QString>* names) {
	// TODO(ashl1future): move the code below to the ImpiGUI
	// The problem in that the ImpiGUI must to be Singleton to support any public method invocation
	// Should a lot of think before do this.
	// The mechanism described in diagram.
	// See also GetGonfigurationPath slot.
	ChooseAccountDialog* dialog = new ChooseAccountDialog(*names);
	if (dialog->exec() == QDialog::Accepted) {
		QVector<QDir>::const_iterator paths_iterator = paths->constBegin();
		for (QVector<QString>::const_iterator it = names->constBegin(); it != names->constEnd(); ++it){
			if (dialog->Name() == *it)
				break;
			else
				++paths_iterator;
		}
		QMetaObject::invokeMethod(sender(), "ChoosedAccount", Qt::QueuedConnection, Q_ARG(QString, paths_iterator->path()));
	} else {
		// TODO(ashl1future):
	}
	delete dialog;
	delete paths;
	delete names;
}

void ImportManager::GetGonfigurationPath(QString home_path) {
	// TODO(ashl1future): move the code below to the ImpiGUI
	// The problem in that the ImpiGUI must to be Singleton to support any public method invocation
	// Should a lot of think before do this.
	// The mechanism described in diagram.
	// See also ChooseAccount slot.
	QString dir_path = QFileDialog::getExistingDirectory(0, "", home_path, QFileDialog::ShowDirsOnly);
	if (dir_path != ""){
		QDir dir;
		// TODO(ashl1future): check for error
		dir.cd(dir_path);
		QMetaObject::invokeMethod(sender(), "GotConfigurationPath", Q_ARG(QString, dir.path()));
	}
}

void ImportManager::SlotInitialized(QString fullname){
	ImportClassInfo* import_class_info = *import_classes_.find(qobject_cast<ImportClass*>(sender()));
	import_class_info->state_ = kReady;
	if (!import_class_info->plugin_->initialized())
		// TODO(ashl1future): Is it ok to const to nonconst cast by this method?
		QMetaObject::invokeMethod((Plugin*)import_class_info->plugin_, "SlotInitialized", Q_ARG(QString, fullname));
}

void ImportManager::ThreadStarted() {
	ImportClassThread* sender_thread = qobject_cast<ImportClassThread*>(sender());
	ImportClassInfo* ic = *threads_.find(sender_thread);
	ic->import_class_->moveToThread(sender_thread);
	QMetaObject::invokeMethod(ic->import_class_, "Init", Q_ARG(QString, ic->plugin_->fullpath()));
}

void ImportManager::DestroyImportClass(ImportClassInfo* import_class_info) {
	plugin_import_classes_.find(import_class_info->plugin_)->removeAll(import_class_info);
	import_classes_.remove(import_class_info->import_class_);
	ImportClassThread* thread = import_class_info->thread_;
	threads_.remove(thread);
	//QMetaObject::invokeMethod(import_class_info->import_class_, "deleteLater");
	QMetaObject::invokeMethod(thread, "quit");
	delete thread;
}

///////////////////////////////////////////////////////////

Plugin::Plugin(QObject* parent/* = NULL*/):QObject(parent){
	initialized_ = false;
}

Plugin::~Plugin(){
	Unload();
}

const QString& Plugin::fullname() const {
	return fullname_;
}

const QString& Plugin::fullpath() const {
	return fullpath_;
}

bool Plugin::can_init_from_file() const {
	// TODO(ashl1future):
	return false;
}

bool Plugin::can_init_from_conf_path() const {
	// TODO(ashl1future):
	return true;
}

bool Plugin::initialized() const {
	return initialized_;
}

void Plugin::GetClientAccounts(const QDir& path) const {
	ImportManager::instance()->GetClientAccounts(this, path);
}

void Plugin::Load(const QString& name) {
	QDir dir(QCoreApplication::applicationDirPath());
	if (!dir.cd("plugins")) {
		; // TODO(ashl1future): rewrite without throw: throw Error(1);
	}
	// TODO(ashl1future): add file extension depend on running OS
	fullpath_ = dir.absoluteFilePath(name);

	ImportManager::instance()->ImportOnline(this);
}

void Plugin::Unload() const{
	// TODO(ashl1future): To realize this, we should understand how plugin will be unloaded (by ImportManager)
	// - see TODO in ImportClass
	// And how we stop (not unload) the ImportClass as we shouldn't has access to that class without ImportManager
}

void Plugin::SlotInitialized(QString fullname){
	initialized_ =  true;
	fullname_ = fullname;
	emit Initialized();
}

void Plugin::ImportOnline() {
	ImportManager::instance()->ImportOnline(this);
}

///////////////////////////////////////////////////////////

Plugins::Plugins(ImpiGUI* ui, QObject* parent/* = NULL*/):QObject(parent) {
	this->ui = ui;
}

Plugins::~Plugins(){
	plugins_.clear();
}

const Plugin* Plugins::GetPluginByName(const QString& fullname) const {
	QMap< QString, const Plugin* >::const_iterator plugins_iterator = plugins_.constBegin();
	for (; plugins_iterator != plugins_.constEnd(); ++plugins_iterator)
		if ((*plugins_iterator)->fullname() == fullname)
			break;
	if (plugins_iterator == plugins_.constEnd())
		return NULL;
	else
		return *plugins_iterator;
}

void Plugins::InitializeAll(){
	// TODO(ashl1future): check filename ( use only without extension) and plugin version too
	// TODO(ashl1future): scan all files
	QString str("libimpi-skype-linux.so");
	Load(str);
}

const Plugin* Plugins::Load(const QString& name) {
	if (plugins_.find(name) == plugins_.end()) {
		Plugin* plugin = new Plugin(this);
		plugins_[name] = plugin;

		connect(plugin, SIGNAL(Initialized()), ui, SLOT(PluginChanged()));

		plugin->Load(name);
		return plugin;
	} else {
		return plugins_[name];
	}
}

void Plugins::Unload(Plugin* plugin){
	plugins_.remove(plugins_.key(plugin));
	plugin->deleteLater();

}

QList<const Plugin*> Plugins::plugins() const{
	return plugins_.values();
}
