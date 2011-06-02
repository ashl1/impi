/*
 * plugins.h
 *
 *  Created on: 23.04.2011
 *      Author: ashl1
 */

#ifndef PLUGINS_H_
#define PLUGINS_H_

#include <QMap>
#include <QObject>
#include <QVector>
#include <QPluginLoader>
#include <QThread>
#include "ui_impi.h"
#include "plugin-interface.h"

class ImportThread;
class Plugin;

// Represents thread class which works with plugin. Plugin loads inside this class.
// This class doing all work to import messages/open files and when finished
// emit signals and get the objects to the another thread.
//
// TODO(ashl1future): Should (or not?) use events and possible to use QMetaObject::invokeMethod.
// and we shouldn't(?) send any data by signal
class ImportClass: public QObject {
	Q_OBJECT
public:
	ImportClass(QObject* parent = NULL): QObject(parent) {};
	~ImportClass(){};

	// This method continue importing procedure. This method run from GotConfigurationPath
	// or from ImportManager::GetGonfigurationPath slot.
	// As this is final actions, in this method PluginInterface::GetNextMessagesOrDie will be executed - starts loop.
	// directory_path - the directory path import from
	Q_INVOKABLE void ChoosedAccount(QString directory_path);
	// This method should be invoked after got configuration path from the GUI to import from
	// In this method ChooseAccount signal is emitted if accounts more than one.
	// The *pathes and *names QVector are allocated in this method and should be freed in ImportManager (in ChooseAccount slot)
	// configuration_path - the string represents configuration path (dir or file) with native separators to import from
	Q_INVOKABLE void GotConfigurationPath(QString configuration_path);
	// Load plugin, initialize inner structures
	// After initialization, emit signal Initialized
	// fullpath - full system path for plugin library
	Q_INVOKABLE void Init(QString plugin_fullpath);
	// TODO(ashl1future): change the description. The method below should be used in any case:
	//   ImportOnline, Import only or OpenFile
	// Start import all history.
	// This method interact with ImpiGUI through ImportManager class
	Q_INVOKABLE void Import();

signals:
	// The signal is emitted in GotConfigurationPath method.
	// This signal decide to choose user account are presented in messenger in GUI.
	// If only one account presented, the signal will not be emitted.
	// pathes - the absolute directory pathes for the accounts
	// names - the names should be showed to the user for choosing
	void ChooseAccount(QVector<QDir>* pathes, QVector<QString>* names);
	// Stop all works and quit from the thread.
	void Finish();
	// The signal is emitted after successfully initialization.
	// Return full name of plugin got from plugin and using in GUI and so on. For addition information see plugin-interface.h
	void Initialized(QString fullname);
	// The signal is emitted when decide for GUI to choose configuration path for importing from.
	// After choosing GotConfigurationPath method should be invoked.
	// home_path - the string represents home path with native separators ('/' or '\')
	void GetGonfigurationPath(QString home_path);

private:
	QPluginLoader* plugin_loader_;
	PluginInterface* plugin_;
};

// The class represent the thread for ImportClass class. It's use only for thread.
// ImportClass should be moved to this thread (class)
class ImportClassThread: public QThread {
	Q_OBJECT
protected:
	// Start the thread
	void run();
};

// Class is a singleton pattern to provide one global point to access plugin by managing ImportClasses.
// For every plugin there are at least one ImportClass: each ImportClass for unique pair <Plugin, Action>
// when Action may be: kImportOnline, kImportFile and kOpenFile. Additional information read in methods
// of the class.
// When load plugin from Plugin class, we should use ImportOnline method. Plugin will be initialized (and ImpiGUI)
// but import may not be started.
// As ImportClass is a thread, this class collaborates with ImportClasses by signals.
// TODO(ashl1future): Should we use at least one ImipiClass with plugin initialized (with kImportOnline) and will not unload
// every time when online functionality will be disabled by user to avoid change in library when program is running.
// In this case we need slot (such as Stop) to stop any actions in ImpiGUI but don't unload plugin.
// One possibility to unload all plugin instances is user manually reload plugins.
class ImportManager: public QObject {
	Q_OBJECT
public:
	ImportManager(QObject* parent = NULL):QObject(parent){};
	~ImportManager();

	// Create if needed the ImportClass to work with all history of messenger.
	// After importing all messages the ImportClass continue working in online (if plugin provide this):
	// when the message comes to the messenger the event (not Qt event) will be as quick as possible generated
	// and our program should update DB and GUI
	void ImportOnline(const Plugin* plugin);

	// Create if needed the ImportClass to work with history from one file (if plugin provide this functionality).
	// After importing to the DB, the ImportClass will be destroyed.
	void ImportFile(const Plugin* plugin);

	// Create if needed the ImportClass to work with history from one file (if plugin provide this functionality).
	// Just open and show the content of the file in the GUI, not import to the DB.
	// After send content to the GUI (or another receiver), the ImportClass will be destroyed.
	void OpenFile(const Plugin* plugin);

	// Query for pathes and names of user accounts on the same computer.
	void GetClientAccounts(const Plugin* plugin, const QDir& path);

	// Return the instance of the ImportManager class
	static ImportManager* instance();

public slots:
	// This slot is emitted when ImportClass decide to choose user account are presented in messenger in GUI.
	// If only one account presented, the signal will not be emitted.
	// paths - the absolute directory paths for the accounts
	// names - the names should be showed to the user for choosing
	void ChooseAccount(QVector<QDir>* paths, QVector<QString>* names);
	// The slot is emitted when ImportClass decide for GUI to choose configuration path for importing from
	// home_path - the string represents home path with native separators ('/' or '\')
	void GetGonfigurationPath(QString home_path);
	// The slot is emitted when any ImportClass will be initialized
	// fullname - full name of plugin got from plugin and using in GUI and so on. For addition information see plugin-interface.h
	void SlotInitialized(QString fullname);
	// This slot should be emitted when ImportClassThread started to move ImportClass to it
	void ThreadStarted();

private:
	// Read the comment to the class
	enum Actions {
		kImportOnline, kImportFile, kOpenFile
	};

	// States of the ImportClass
	enum States {
		kInitializing, kReady, kRunning
	};

	static QPointer<ImportManager> instance_;

	// The struct that represent ImportClass state and information
	struct ImportClassInfo{
		Actions action_;
		ImportClass* import_class_;
		const Plugin* plugin_;
		States state_;
		ImportClassThread* thread_;
	};

	// Destroy ImportClass, stop and destroy ImportClassThread and remove its pointers from containers
	void DestroyImportClass(ImportClassInfo* import_class_info);

	// The ImportClasses in the map
	// This provides the functionality to make only one ImportClass for unique pair <Plugin, Action>.
	// The variable is used in ImportOnline, ImportFile, OpenFile methods.
	QMap<const Plugin*, QList<ImportClassInfo*> > plugin_import_classes_;

	// The map of ImportClassInfo. It's used in GetImportClassInfoByClass() method
	QMap<const ImportClass*, ImportClassInfo*> import_classes_;

	// The map of the threads (ImportClassThread)
	QMap<const ImportClassThread*, ImportClassInfo*> threads_;
};

// The class implement Plugin abstraction for using in inner program structure
// TODO(ashl1future): Think about OOP for this class. This class should be unaccessable to Load and Unload
// methods withot Plugins class
class Plugin: public QObject {
	Q_OBJECT
public:
	Plugin(QObject* parent = NULL);
	~Plugin();

	const QString& fullname() const;
	const QString& fullpath() const;
	bool can_init_from_file() const;
	bool can_init_from_conf_path() const;
	bool initialized() const;

	// Query for pathes and names of user accounts on the same computer. Determine it by plugin (see plugin-interface.h)
	// Should execute only once until GotAccounts emitted
	void GetClientAccounts(const QDir& path) const;

	// TODO(ashl1future): should hide this method for all classes not Plugins
	// TODO(ashl1future): load only if not loaded, unload only if loaded
	// TODO(ashl1future): Is they really should be const?
	// Load plugin. These functions must called only from Plugins class!
	// name - plugin library's name (without extension - it's depend on running OS)
	void Load(const QString& name);
	void Unload() const;

signals:
	// This signals will be emitted to the GUI only first time plugin initialize (or reinitialize)
	// but not when creating another plugin threads ImportClass
	void Initialized();

public slots:
	// This slot should be emitted from GUI (ImpiGUI class).
	// Emitting this tell to start import online procedure
	void ImportOnline();
	// This slot should be emitted once when main plugin (kImportOnline) was initialized.
	// Or when reinitialized by reloading plugin.
	// fullname - full name of plugin got from plugin and using in GUI and so on. For addition information see plugin-interface.h
	void SlotInitialized(QString fullname);

private:
	// Full name of plugin got from plugin and using in GUI and so on. For addition information see plugin-interface.h
	QString fullname_;
	// Full system path for plugin library
	QString fullpath_;
	// If true, plugin could import from one file and open one file to show the user.
	bool can_init_from_file_;
	// If true, plugin could import history from messenger configuration path.
	bool can_init_from_conf_path_;
	// If true, plugin should be notified by SlotInitialized() slot that real plugin code was initialized
	bool initialized_;
};

// Container for Plugins. Provide one point to access for plugins in program, so it's recommended to use this class
// to acces for plugins.
class Plugins: public QObject{
	Q_OBJECT
public:
	Plugins(ImpiGUI* ui, QObject* parent = NULL);
	~Plugins();

	// Return the plugin by its full name (See plugin-interface.h for description of fullname).
	// Return NULL if not found.
	// fullname - Full name of plugin got from plugin and using in GUI and so on.
	const Plugin* GetPluginByName(const QString& fullname) const;

	// Initialize all plugins from standard directory ${Program}/"plugins"
	void InitializeAll();

	// Load plugin. Do it only if not already loaded.
	// name - plugin library's name (without extension - it's depend on running OS)
	// Return pointer to the plugin. But plugin itself cannot be used immediately because it's in the another thread
	// use Initialized() signal of the plugin to understand that at least one plugin was loaded.
	// When import not online or open you should use another behavior, please read Plugin::Initialized().
	const Plugin* Load(const QString& name);

	void Unload(Plugin* plugin);

	// Return all plugins in QList
	QList<const Plugin*> plugins() const;

private:
	// Container for Plugins. First argument QString is library name (just only filename without extension) of plugin library
	QMap<QString, const Plugin*> plugins_;

	// The pointer to the ImpiGUI ui class, It is used for make the connections between plugin and GUI
	// For example, when plugin will be initialized, GUI changes the state
	ImpiGUI* ui;
};

#endif /* PLUGINS_H_ */
