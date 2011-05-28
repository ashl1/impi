/*
 * plugins.cpp
 *
 *  Created on: 23.04.2011
 *      Author: ashl1
 */

#include "plugins.h"
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QString>

#include "plugins.h"

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

///////////////////////////////////////////////////////////

void ImportClass::run() {
	exec();
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

void ImportClass::Import(QString fullpath) {
	// TODO(ashl1future)
}

void ImportClass::Finish() {
	quit();
}

///////////////////////////////////////////////////////////

void ImportManager::ImportOnline(const Plugin* plugin) {
	if (plugin_import_classes_.find(plugin) == plugin_import_classes_.end()) {
		// There are no ImportClasses for this plugin
		ImportClassInfo import_class_info;
		import_class_info.import_class_ = new ImportClass(this);
		import_class_info.plugin = plugin;
		import_class_info.action_ = kImportOnline;
		import_class_info.state_ = kInitializing;

		import_classes_[import_class_info.import_class_] = import_class_info;
		plugin_import_classes_[plugin].append(import_class_info);

		// TODO(ashl1future): connect all needed signals from ImportClass
		connect(import_class_info.import_class_, SIGNAL(Initialized(QString)), this, SLOT(SlotInitialized(QString)));

		import_class_info.import_class_->start(QThread::LowestPriority);
		QMetaObject::invokeMethod(import_class_info.import_class_, "Init", Q_ARG(QString, plugin->fullpath()));
	} else {
		// There is at least one ImportClass for this plugin
		// TODO(ashl1future): find the ImportClass by action_, check state_
		// If it's kReady, apply actions, else ???
		// If not found, then we should create another on ImportClass
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

void ImportManager::SlotInitialized(QString fullname){
	ImportClassInfo import_class_info = GetImportClassInfoByClass(qobject_cast<ImportClass*>(sender()));
	import_class_info.state_ = kReady;
	if (!import_class_info.plugin->initialized())
		// TODO(ashl1future): Is it ok to const to nonconst cast by this method?
		QMetaObject::invokeMethod((Plugin*)import_class_info.plugin, "SlotInitialized", Q_ARG(QString, fullname));
}

ImportManager::ImportClassInfo ImportManager::GetImportClassInfoByClass(const ImportClass* import_class) const {
	return (*import_classes_.find(import_class));
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
