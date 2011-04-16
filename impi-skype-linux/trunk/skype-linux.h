/*
 * skype-linux.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef SKYPELINUX_H_
#define SKYPELINUX_H_

#include <QObject>
#include "../impi/client-interface.h"

class PluginSkypeLinux : public QObject, public ClientInterface {
Q_OBJECT
Q_INTERFACES(ClientInterface)

private:

public:
	virtual ~PluginSkypeLinux();

	virtual bool CanInitFromFile() const;

	virtual void InitFromConfPath(QDir* path);

};

#endif /* SKYPELINUX_H_ */
