//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    plugin_networking.h
  \brief   C++ Interface: Plugin_Networking
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#ifndef __PluginNetworking_HPP
#define __PluginNetworking_HPP

// Qt
#include <QtDebug>
// C++
#include <string.h>
#include <iomanip>
// Local
#include "../Plugin.hpp"
#include "Frame.hpp"
#include "practicalsocket.h"
#include "timer.h"

class PluginNetworking:public Plugin {
 public:
	PluginNetworking();
	PluginNetworking(int _n);

	virtual int process(Item * item);

 public:
	unsigned short destPort;
	int n;
	UDPSocket sock;
	 std::string destAddress;
	 std::string sendString;
};

#endif
