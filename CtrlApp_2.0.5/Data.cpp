// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ctrlapp.h"
#include "Data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CData::CData()
{
	// Clear accumulator memory
	/*
	for (int i = 0 ; i < 5 ; i++){
		for (int j = 0 ; j < 8 ; j++){
			total_time[i][j] = 0.0;
			total_data[i][j] = 0;
			average_latency[i][j] = 0.0;
			max_latency[i][j] = 0.0;
			number[i][j] = 0;
		}
	}
	*/
}

CData::~CData()
{

}

void CData::AddResult(int mode, DWORD length, int blocksize, double rate, double latency)
{
	/* Resolution and data representation
		length is in kbyte
		blocksize is in kbyte
		rate is in Mbyte/s
		latency is in ms
	*/

	//AfxMessageBox("Arrived at add data function");
	//CString str;
	if((length == 0) || (blocksize == 0) || (rate == 0.0)) return; // This can happen when client is closed locally
	int block_size_index = 0;

	switch (blocksize) {
		case 1:  block_size_index = 0 ; break ;
		case 2:  block_size_index = 1 ; break ;
		case 4:  block_size_index = 2 ; break ;
		case 8:  block_size_index = 3  ; break ;
		case 16: block_size_index = 4  ; break ;
		case 32: block_size_index = 5  ; break ;
		case 64: block_size_index = 6  ; break ;
		case 128: block_size_index = 7  ; break ;
		case 256: block_size_index = 8  ; break ;
		case 512: block_size_index = 9  ; break ;
		case 1024: block_size_index = 10  ; break ;
	}

	DWORD n_blocks = length >> block_size_index /* / (DWORD)blocksize */;
	double data = (double)(n_blocks * blocksize);
	double access_time = data / rate;
	/*
		n_blocks is integer and can range from 1 to about 41E6 - OK for DWORD.
		data is in kbyte, but floating point. Each access event could be between 1 and 41M.
		That's no problem for each individual one but the accumulation is limited.
		With 'double' the numerical resolution is about 1 part in 10^15. That would
		give about 24 million access event (of 40 Gbyte each) before the added data 
		began to get ignored. That's more than enough, but other representations would 
		not do.
		access_time is in ms, ie. kbyte / Mbyte per second (= kbtye per ms). That's a
		much smaller problem than the data because it is likely to be between about
		2 and 1000 only.
	*/
	//str.Format("Blocksize = %d, Index = %d", blocksize, block_size_index);
	//AfxMessageBox((LPCTSTR)str);


	// Accumulate data ...
	// Individual 
	total_time[mode][block_size_index] += access_time;
	total_data[mode][block_size_index] += data;
	// All modes
	total_time[4][block_size_index] += access_time;
	total_data[4][block_size_index] += data;
	// All blocksizes
	total_time[mode][11] += access_time;
	total_data[mode][11] += data;
	// Overall
	total_time[4][11] += access_time;
	total_data[4][11] += data;

	// Individual 
	average_latency[mode][block_size_index] += latency;
	number[mode][block_size_index]++;
	// All modes
	average_latency[4][block_size_index] += latency;
	number[4][block_size_index]++;
	// All blocksizes
	average_latency[mode][11] += latency;
	number[mode][11]++;
	// Overall
	average_latency[4][11] += latency;
	number[4][11]++;

	// Individual 
	double * p = &max_latency[mode][block_size_index];
	if ( latency > *p) *p = latency;

	// All modes
	p = &max_latency[4][block_size_index];
	if ( latency > *p) *p = latency;

	// All blocksizes
	p = &max_latency[mode][11];
	if ( latency > *p) *p = latency;

	// Overall
	p = &max_latency[4][11];
	if ( latency > *p) *p = latency;
}

void CData::Printout(CFile * file)
{
	CString output_buffer;
	CString mode_str[5] = {"seq_rd","rand_rd","seq_wr","rand_wr","All"};
	int i, j, buf_length;

	// tabs replaced by commas for Excel compatibility - 02/07/04

	/* Tabbed version
	//**************** Print out average transfer rates ************************

	output_buffer.Format("Results summary - average transfer rates, Mbytes/s\n\tBlock size\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	output_buffer.Format("Mode\t    1k\t    2k\t    4k\t    8k\t   16k\t   32k\t   64k\t   All\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	for (i = 0 ; i < 5 ; i++){
		output_buffer.Format("%s\t", mode_str[i]);
		buf_length = output_buffer.GetLength();
		file->Write(output_buffer, buf_length);

		for (j = 0 ; j < 8 ; j++){
			if(total_data[i][j] == 0.0){
				output_buffer.Format("%7.2f\t", 0.0);
			}
			else{
				output_buffer.Format("%7.2f\t", total_data[i][j] / total_time[i][j]);
			}
			buf_length = output_buffer.GetLength();
			file->Write(output_buffer, buf_length);
		}
		file->Write("\n", 1);
	}

	//*************** Print out average latency ***********************

	output_buffer.Format("\nResults summary - average latency, ms\n\tBlock size\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	output_buffer.Format("Mode\t    1k\t    2k\t    4k\t    8k\t   16k\t   32k\t   64k\t   All\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	for (i = 0 ; i < 5 ; i++){
		output_buffer.Format("%s\t", mode_str[i]);
		buf_length = output_buffer.GetLength();
		file->Write(output_buffer, buf_length);

		for (j = 0 ; j < 8 ; j++){
			if(average_latency[i][j] == 0.0){
				output_buffer.Format("%7.1f\t", 0.0);
			}
			else{
				output_buffer.Format("%7.1f\t", average_latency[i][j] / (double) number[i][j]);
			}
			buf_length = output_buffer.GetLength();
			file->Write(output_buffer, buf_length);
		}
		file->Write("\n", 1);
	}

	//**************** Print out maximum latency ***********************

	output_buffer.Format("\nResults summary - maximum latency, s\n\tBlock size\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	output_buffer.Format("Mode\t    1k\t    2k\t    4k\t    8k\t   16k\t   32k\t   64k\t   All\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	for (i = 0 ; i < 5 ; i++){
		output_buffer.Format("%s\t", mode_str[i]);
		buf_length = output_buffer.GetLength();
		file->Write(output_buffer, buf_length);

		for (j = 0 ; j < 8 ; j++){
			if(max_latency[i][j] == 0.0){
				output_buffer.Format("%7.3f\t", 0.0);
			}
			else{
				output_buffer.Format("%7.3f\t", max_latency[i][j] / 1000.0);
			}
			buf_length = output_buffer.GetLength();
			file->Write(output_buffer, buf_length);
		}
		file->Write("\n", 1);
	}
	//End of tabbed version 
	*/

	/* Comma version */
	//**************** Print out average transfer rates ************************

	output_buffer.Format("Results summary\nAverage transfer rates, Mbytes/s\n,Block size\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	output_buffer.Format("Mode,1k,2k,4k,8k,16k,32k,64k,128k,256k,512k,1024k,All\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	for (i = 0 ; i < 5 ; i++){
		output_buffer.Format("%s,", mode_str[i]);
		buf_length = output_buffer.GetLength();
		file->Write(output_buffer, buf_length);

		for (j = 0 ; j < 12 ; j++){
			if(total_time[i][j] == 0.0){
				output_buffer.Format("%7.2f,", 0.0);
			}
			else{
				output_buffer.Format("%7.2f,", total_data[i][j] / total_time[i][j]);
			}
			buf_length = output_buffer.GetLength();
			file->Write(output_buffer, buf_length);
		}
		file->Write("\n", 1);
	}

	//*************** Print out average latency ***********************

	output_buffer.Format("\nAverage latency,ms\n,Block size\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	output_buffer.Format("Mode,1k,2k,4k,8k,16k,32k,64k,128k,256k,512k,1024k,All\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	for (i = 0 ; i < 5 ; i++){
		output_buffer.Format("%s,", mode_str[i]);
		buf_length = output_buffer.GetLength();
		file->Write(output_buffer, buf_length);

		for (j = 0 ; j < 12 ; j++){
			if(number[i][j] == 0){
				output_buffer.Format("%7.1f,", 0.0);
			}
			else{
				output_buffer.Format("%7.1f,", average_latency[i][j] / (double) number[i][j]);
			}
			buf_length = output_buffer.GetLength();
			file->Write(output_buffer, buf_length);
		}
		file->Write("\n", 1);
	}

	//**************** Print out maximum latency ***********************

	output_buffer.Format("\nMaximum latency,s\n,Block size\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	output_buffer.Format("Mode,1k,2k,4k,8k,16k,32k,64k,128k,256k,512k,1024k,All\n");
	buf_length = output_buffer.GetLength();
	file->Write(output_buffer, buf_length);

	for (i = 0 ; i < 5 ; i++){
		output_buffer.Format("%s,", mode_str[i]);
		buf_length = output_buffer.GetLength();
		file->Write(output_buffer, buf_length);

		for (j = 0 ; j < 12 ; j++){
			output_buffer.Format("%7.3f,", max_latency[i][j] / 1000.0);
			buf_length = output_buffer.GetLength();
			file->Write(output_buffer, buf_length);
		}
		file->Write("\n", 1);
	}
	//End of comma version 
}

void CData::Clear()
{
	for (int i = 0 ; i < 5 ; i++){
		for (int j = 0 ; j < 12 ; j++){
			total_time[i][j] = 0.0;
			total_data[i][j] = 0;
			average_latency[i][j] = 0.0;
			max_latency[i][j] = 0.0;
			number[i][j] = 0;
		}
	}
}
