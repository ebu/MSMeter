// Header.cpp: implementation of the Header class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ctrlapp.h"
#include "Header.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Header::Header()
{
#ifdef _DEBUG						
	//Debug defaults
	testfilepath = "c:\\";
	//testfilepath = "c:\\dump\\" ;
	maxdelay = 500;
#else
	testfilepath = "not set";
	//testfilepath = "c:\\dump\\";
	maxdelay = 0;
#endif
	maxnum = 1;
	maxlength = 1024;
	pcwrite = 0;
	pcrand = 0;
	verbose = TRUE;
	sharing = FALSE;
	changed = TRUE;
	
	//MS Meter 2 block size selection default values 1K to 64K
	lstblocksize[1] = TRUE;
	lstblocksize[2] = TRUE;
	lstblocksize[3] = TRUE;
	lstblocksize[4] = TRUE;
	lstblocksize[5] = TRUE;
	lstblocksize[6] = TRUE;
	lstblocksize[7] = TRUE;
	lstblocksize[8] = FALSE;
	lstblocksize[9] = FALSE;
	lstblocksize[10] = FALSE;
	lstblocksize[11] = FALSE;
	directio = FALSE;

	//MS Meter 3 default values
	per_frame = FALSE;
	frame_size = 65536;
	ntp_stamps = FALSE;
	ntp_address = "";

}

Header::~Header()
{

}

