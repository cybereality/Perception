/***************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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
****************************************************************/

/*** D3DPERF super method prototypes ***/
typedef int   (WINAPI *LPD3DPERF_BeginEvent)       (D3DCOLOR col, LPCWSTR wszName);
typedef int   (WINAPI *LPD3DPERF_EndEvent)         (void);
typedef void  (WINAPI *LPD3DPERF_SetMarker)        (D3DCOLOR col, LPCWSTR wszName);
typedef void  (WINAPI *LPD3DPERF_SetRegion)        (D3DCOLOR col, LPCWSTR wszName);
typedef BOOL  (WINAPI *LPD3DPERF_QueryRepeatFrame) (void);
typedef void  (WINAPI *LPD3DPERF_SetOptions)       (DWORD dwOptions);
typedef DWORD (WINAPI *LPD3DPERF_GetStatus)        (void);

/*** D3DPERF super methods ***/
LPD3DPERF_BeginEvent       g_pD3DPERF_BeginEvent_Super       = NULL;
LPD3DPERF_EndEvent         g_pD3DPERF_EndEvent_Super         = NULL;
LPD3DPERF_SetMarker        g_pD3DPERF_SetMarker_Super        = NULL;
LPD3DPERF_SetRegion        g_pD3DPERF_SetRegion_Super        = NULL;
LPD3DPERF_QueryRepeatFrame g_pD3DPERF_QueryRepeatFrame_Super = NULL;
LPD3DPERF_SetOptions       g_pD3DPERF_SetOptions_Super       = NULL;
LPD3DPERF_GetStatus        g_pD3DPERF_GetStatus_Super        = NULL;

/*** D3DPERF methods wrapped ***/
int   WINAPI D3DPERF_BeginEvent(D3DCOLOR col, LPCWSTR wszName) { return g_pD3DPERF_BeginEvent_Super(col, wszName); }
int   WINAPI D3DPERF_EndEvent(void) { return g_pD3DPERF_EndEvent_Super(); }
void  WINAPI D3DPERF_SetMarker(D3DCOLOR col, LPCWSTR wszName) { g_pD3DPERF_SetMarker_Super(col, wszName); }
void  WINAPI D3DPERF_SetRegion(D3DCOLOR col, LPCWSTR wszName) { g_pD3DPERF_SetRegion_Super(col, wszName); }
BOOL  WINAPI D3DPERF_QueryRepeatFrame(void) { return g_pD3DPERF_QueryRepeatFrame_Super(); }
void  WINAPI D3DPERF_SetOptions(DWORD dwOptions) { g_pD3DPERF_SetOptions_Super(dwOptions); }
DWORD WINAPI D3DPERF_GetStatus(void) { return g_pD3DPERF_GetStatus_Super(); }