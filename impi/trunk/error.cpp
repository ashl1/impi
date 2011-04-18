/*
 * error.cpp
 *
 *  Created on: Apr 17, 2011
 *      Author: ashl1
 */

#include "error.h"

Error::Error(quint16 type, QString text):type(type){
	this->text = text;
}

Error::Error(quint16 type):type(type){}
