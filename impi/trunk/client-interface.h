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
	bool CanInitFromFile();

	void InitFromConfPath(QDir* path);
};

Q_DECLARE_INTERAFCE(ClientInterface, "org.impi.ClientInterface/0.1")

#endif /* CLIENTINTERFACE_H_ */
