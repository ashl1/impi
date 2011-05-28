/*
 * skype-linux.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef SKYPELINUX_H_
#define SKYPELINUX_H_

#include <QDir>
#include <QObject>
#include <QVector>
#include "../impi/plugin-interface.h"

namespace SkypeLinux {

	class AccountSkypeLinux: public PluginAccount{
	public:
		// get number in qvector
		virtual quint32 GetNumber() const;

		virtual PluginProtocol* GetProtocol() const;
		virtual PluginUser* GetUser() const;
	};

	class ChatSkypeLinux: public PluginChat{
	public:
		virtual QDateTime GetStartTime() const;
		virtual QDateTime GetModifiedTime() const;
	};

	class MessageSkypeLinux: public PluginMessage{
	public:
		virtual QDateTime GetTime() const;
		virtual QString GetBody() const;

		virtual PluginAccount* GetAccount() const;
		virtual PluginClient* GetClient() const;
	};

	class ProtocolSkypeLinux: public PluginProtocol{
	private:
		QString name;
		quint32 number;
	public:
		ProtocolSkypeLinux(QString name, quint32 number):name(name), number(number){};

		virtual QString GetName() const;
		// get number in qvector
		virtual quint32 GetNumber() const;
	};

	class UserSkypeLinux: public Impi::PluginUser{

	};


	class PluginSkypeLinux: public QObject, public Impi::PluginInterface {
	Q_OBJECT
	Q_INTERFACES(Impi::PluginInterface)

	private:
		QVector<Impi::PluginAccount*> accounts;
		QVector<Impi::PluginChat*> chats;
		QVector<Impi::PluginMessage*> messages;
		QVector<Impi::PluginProtocol*> protocols;
		QVector<Impi::PluginUser*> users;

	public:
		virtual ~PluginSkypeLinux();

		virtual QString Version() const;
		virtual QString FullName() const;
		virtual QString MajorName() const;
		virtual QString MinorName() const;
		virtual bool CanInitFromFile() const;

		virtual void InitializeEvents(QObject* pluginDummy);

		virtual void GetClientAccounts(const QDir& path, QVector<QDir>& pathes, QVector<QString>& names) const;
		virtual void InitFromConfPath(const QDir& path);
		// should rewrite to use Error class. Do it when make main Impi library
		virtual void InitFromFile(const QDir& path, const QString& filename);

		virtual const Impi::PluginClient* GetClient() const;

		virtual void InitProtocols();
		virtual const QVector<Impi::PluginProtocol*>* GetProtocols() const;

		virtual void InitUsers();
		virtual const QVector<Impi::PluginUser*>* GetUsers() const;

		virtual void InitAccounts();
		virtual const QVector<Impi::PluginAccount*>* GetAccounts() const;

		virtual void InitNextChats(quint16 limit, QDateTime& time);
		virtual const QVector<Impi::PluginChat*>* GetChats() const;

		virtual void InitNextMessages(quint16 limit, QDateTime& time);
		virtual const QVector<Impi::PluginMessage*>* GetMessages() const;

	signals:
		void Initialized();
	};
} // namespace SkypeLinux

#endif /* SKYPELINUX_H_ */
