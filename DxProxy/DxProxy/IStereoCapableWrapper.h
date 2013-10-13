/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain, Denis Reischl, Neil Schneider 
  and Joshua Brown 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#ifndef STEREOCAPABLEWRAPPER_H_INCLUDED
#define STEREOCAPABLEWRAPPER_H_INCLUDED

template <class T>

/**
* Template class, stereo capable wrapper prototype. 
*/
class IStereoCapableWrapper
{
public:
	virtual ~IStereoCapableWrapper() {}

	/*** IStereoCapableWrapper public methods ***/
	virtual bool IsStereo() = 0;
	virtual T*   getActualMono() = 0;
	virtual T*   getActualLeft() = 0;
	virtual T*   getActualRight() = 0;
};

#endif