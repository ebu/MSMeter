// Header.h: interface for the Header class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HEADER_H__A571865D_A1A1_4402_8462_59FE8B160A8F__INCLUDED_)
#define AFX_HEADER_H__A571865D_A1A1_4402_8462_59FE8B160A8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Header  
{
public:
	BOOL sharing;
	Header();
	virtual ~Header();

	BOOL changed;	
	CString testfilepath;
	int maxnum;
	int maxlength;
	int maxdelay;
	int pcwrite;
	int pcrand;
	BOOL verbose;
	BOOL lstblocksize[MAXBLOCKTYPE];
	BOOL directio;

};

#endif // !defined(AFX_HEADER_H__A571865D_A1A1_4402_8462_59FE8B160A8F__INCLUDED_)
