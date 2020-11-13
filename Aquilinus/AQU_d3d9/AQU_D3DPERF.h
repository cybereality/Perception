/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <AQU_D3DPERF.h>
Copyright © 2014 Denis Reischl

This code is private and MUST NOT be set public for any 
reason. This code is intended to be used, changed, compiled 
and its build published only by Denis Reischl.

This code is intended to be used by its author,
Denis Reischl,
for any commercial and non-commercial purpose.

Following persons are granted to read the full Aquilinus
source code for life time :
Neil Schneider, Grant Bagwell and Simon Brown.
If the original author of Aquilinus, Denis Reischl,
discontinues the work on this software, the named persons
automatically inherit all rights to continue this work.

Except where otherwise noted, this work is licensed under :
<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.
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