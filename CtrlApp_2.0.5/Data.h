// Data.h: interface for the CData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__A6F4A6EE_9764_4BFF_A294_4F4DEE33E2AB__INCLUDED_)
#define AFX_DATA_H__A6F4A6EE_9764_4BFF_A294_4F4DEE33E2AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Data arrays for results summary, 5 rows by 12 columns
// rows = read / write measurement types: seq_rd, rand_rd, seq-wr, rand_wr, All
// columns = block size averaged results: 1K, 2K, 4K, 8K, 16K, 32K, 64K, 128K, 256K, 512K, 1M, All 
class CData  
{
public:
	void Clear(void);
	void Printout(CFile * file);
	DWORD number[5][12];
	double max_latency[5][12];
	double average_latency[5][12];
	double total_data[5][12];
	double total_time[5][12];
	void AddResult(int mode, DWORD length, int blocksize, double rate, double latency);
	CData();
	virtual ~CData();

};

#endif // !defined(AFX_DATA_H__A6F4A6EE_9764_4BFF_A294_4F4DEE33E2AB__INCLUDED_)
