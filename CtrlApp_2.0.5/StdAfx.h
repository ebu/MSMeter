// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D0281AAF_2063_4F77_B488_D6115B827F24__INCLUDED_)
#define AFX_STDAFX_H__D0281AAF_2063_4F77_B488_D6115B827F24__INCLUDED_

#define _WIN32_WINNT 0x0601	//Win 7

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

// AFX / MFC threading
//#include <winsock2.h>		
//#include <winsock.h>
//#include <windows.h>
//#include <iostream>

//#define WM_USER_DIALOG_DESTROYED WM_USER + 0x100
#define MAXCLIENTS 200			//Max number of client worker programs allowed at one time
#define MAXBLOCKTYPE 12			//Max number of block size types = 1 to 11
#define MAXPAUSETIME 172800		//Max script pause time in seconds, 172800 seconds = 48 hours
#define MAXNOFILES 999			//Max number of target files on client
#define MAXLENGTH 2097151		//Max length in Kbytes of data chunk for read or write
#define MINLENGTH 2				//Min length in Kbytes of data chunk for read or write
#define MAXDELAY 10000			//Max delay in ms between r/w operations is 10,000ms	
#define MAXSCRIPTCOMMANDS 20000	//Max number of lines allowed in a script file
#define WM_USER_APPLY (WM_USER + 100)

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D0281AAF_2063_4F77_B488_D6115B827F24__INCLUDED_)
