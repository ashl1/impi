/*
 * error.h
 *
 *  Created on: Apr 17, 2011
 *      Author: ashl1
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <QString>

class Error {
	quint16 type;
	QString text;

public:
	Error(quint16 type, QString text);
	Error(quint16 type);
};

/* Types of errors
* 1 - Object cannot support this function. For example: in plugins.
* 2 - Cold not find a file. For example: "plugins" directory
*
 */

#endif /* ERROR_H_ */
