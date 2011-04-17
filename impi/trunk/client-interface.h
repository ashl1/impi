/*
 * client-interface.h
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#ifndef CLIENTINTERFACE_H_
#define CLIENTINTERFACE_H_

class QDir;

class ClientInterface {
public:
	virtual ~ClientInterface() {}

	virtual bool CanInitFromFile() const = 0;

	virtual void InitFromConfPath(QDir* path) = 0;
};

Q_DECLARE_INTERFACE(ClientInterface, "org.impi.ClientInterface/0.1")

#endif /* CLIENTINTERFACE_H_ */
