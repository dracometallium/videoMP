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
  \file    plugin_networking.cpp
  \brief   C++ Implementation: Plugin_Networking
  \author  Guillermo Eduardo Torres, (C) 2012
*/
//========================================================================

#include "PluginNetworking.hpp"

PluginNetworking::PluginNetworking()
{
	destAddress = "10.0.2.255";
	destPort = 9998;
	sendString = "Hello, World!";
}

PluginNetworking::PluginNetworking(int _n)
{
	destAddress = "10.0.2.255";
	destPort = 9998;
	sendString = "Hello, World!";
	n = _n;
}

int PluginNetworking::process(Item * item)
{
	Frame *frame;
	frame = (Frame *) item;
	if (n == 1) {
		for (unsigned int j = 0;
		     j < (*frame->data)[0]->blue_team->patches.size(); ++j) {
			int detected;
			if ((*frame->data)[0]->blue_team->patches[j]->detected)
				detected = 1;
			else
				detected = 0;
			qDebug() << QString::number(GetTimeSec(), 'f',
						    6) << "seconds;" <<
			    "Thread of Processing" << n << ";" << "#Frame" <<
			    (*frame->
			     data)[0]->frameNumber << ";" << "Object" <<
			    (*frame->data)[0]->blue_team->
			    patches[j]->id << ";" << "Detected" << detected <<
			    ";" << "pixel(" << (*frame->data)[0]->blue_team->
			    patches[j]->center.x << "," << (*frame->data)[0]->
			    blue_team->patches[j]->center.
			    y << ");" << "pos(" << (*frame->data)[0]->
			    blue_team->patches[j]->
			    field_pos.x << "," << (*frame->data)[0]->blue_team->
			    patches[j]->field_pos.y << ")mm;" << "orient(" <<
			    QString::number((*frame->data)
					    [0]->blue_team->patches
					    [j]->orientation, 'f',
					    2) << ")rad;" << "orient(" <<
			    QString::number(((*frame->data)[0]->blue_team->
					     patches[j]->orientation * 180 /
					     3.1415), 'f',
					    2) << ")grad;" << "team marker area"
			    << (*frame->data)[0]->blue_team->patches[j]->
			    teamMarker.area << "px;"
			    /* for general use of markers, enable the next "for" loop below and disable this seccion */
			    << "secondary marker 0 center(" <<
			    (*frame->data)[0]->blue_team->patches[j]->
			    markers[0]->center.x << "," << (*frame->data)[0]->
			    blue_team->patches[j]->markers[0]->
			    center.y << ");" << "secondary marker 0 area" <<
			    (*frame->data)[0]->blue_team->patches[j]->
			    markers[0]->area << "px;" <<
			    "secondary marker 1 center(" << (*frame->data)[0]->
			    blue_team->patches[j]->markers[1]->center.
			    x << "," << (*frame->data)[0]->
			    blue_team->patches[j]->markers[1]->
			    center.y << ")px;" << "secondary marker 1 area" <<
			    (*frame->data)[0]->blue_team->patches[j]->
			    markers[1]->area << "px;" <<
			    "secondary marker 2 center(" << (*frame->data)[0]->
			    blue_team->patches[j]->markers[2]->center.
			    x << "," << (*frame->data)[0]->
			    blue_team->patches[j]->markers[2]->
			    center.y << ")px;" << "secondary marker 2 area" <<
			    (*frame->data)[0]->blue_team->patches[j]->
			    markers[2]->area << "px;" <<
			    "secondary marker 3 center(" << (*frame->data)[0]->
			    blue_team->patches[j]->markers[3]->center.
			    x << "," << (*frame->data)[0]->
			    blue_team->patches[j]->markers[3]->
			    center.y << ")px;" << "secondary marker 3 area" <<
			    (*frame->data)[0]->blue_team->patches[j]->
			    markers[3]->area << "px";
		}
	} else if (n == 2)
		qDebug() << QString::number(GetTimeSec(), 'f', 6) << "seconds;"
		    << "Thread of Processing" << n << ";"
		    << "#Frame" << (*frame->data)[0]->frameNumber << ";"
		    << "Object Ball;"
		    << "pixel(" << (*frame->data)[0]->ball->center.x << ","
		    << (*frame->data)[0]->ball->center.y << ");"
		    << "pos(" << (*frame->data)[0]->ball->field_pos.x << ","
		    << (*frame->data)[0]->ball->field_pos.y << ")mm";

	return 0;
}
