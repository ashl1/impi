/*
 * skype-linux.cpp
 *
 *  Created on: Mar 25, 2011
 *      Author: ashl1
 */

#include <qplugin.h>
#include "skype-linux.h"

PluginSkypeLinux::~PluginSkypeLinux(){

}

/* virtual */ bool PluginSkypeLinux::CanInitFromFile() const{
	return false;
}

/* virtual */ void PluginSkypeLinux::InitFromConfPath(QDir* path){
	int a;
	a = 10;
}

Q_EXPORT_PLUGIN2(impi-skype-linux, PluginSkypeLinux)
