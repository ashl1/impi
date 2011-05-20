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
public:
	quint16 type;
	QString text;

public:
	Error(quint16 type, QString text);
	Error(quint16 type);
};

/* Types of errors
* 1 - should reserved for unexpected error
* 1 - Object cannot support this function. For example: in plugins.
* 2 - Could not find a file/directory. For example: "plugins" directory
* 3 - Cannot unload plugin
* 4 - This is not valid path
* 5 - The resource is busy
* 6 - Cannot load the file
 */

#endif /* ERROR_H_ */
