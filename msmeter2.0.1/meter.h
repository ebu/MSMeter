/******************************************************************************
 ** Media storage meter
 ** Header file
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <errno.h>
#include "stdint.h"

#if defined (METER_OS_WIN32)
  #include "stdafx.h"
  #include <string>
#endif

#if defined (METER_OS_LINUX)
  #include <fcntl.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <sys/types.h>
  #include <termios.h>
#endif

using namespace std;

typedef unsigned long ulong;

struct TimeStore {
  double upper,lower;
};

// Pick which I/O library should be used
#if defined (METER_FILE_CLIB)
  typedef FILE *  FileRef; // Standard C library
#elif defined (METER_FILE_UNIX)
  typedef int     FileRef; // Unix system calls
//  typedef FILE * File64; // Unix system calls
#elif defined (METER_FILE_FSTREAM)
  typedef fstream FileRef; // C++ fstream
  typedef LONGLONG off64_t; // file offset type
  //typedef __int64 streamoff;
  //typedef __int64 streamsize;
#elif defined (METER_FILE_WIN32)
  typedef HANDLE  FileRef; // Win32 API
    typedef LONGLONG off64_t; // MFC CFile file offset type
#elif defined (METER_FILE_MFC)
  typedef CFile FileRef; // MFC CFile
    typedef LONGLONG off64_t; // MFC CFile file offset type
#endif

#define MAXBLOCKSIZETYPE 12	//Maxx nuber of block size types 1 to 11

void delay_ms(int ms);
void main_prog(void);
void delay(int t);
void delay_ms(int ms);
void startTimer(TimeStore &return_value);
double stopTimer(TimeStore &start_value);
//double read(FileRef &f, ulong addressInfo, ulong no_blocks, ulong block_size, bool random);
double read64(FileRef &f, uint64_t addressInfo, ulong no_blocks, ulong block_size, bool random);
//double write(FileRef &f, ulong addressInfo, ulong no_blocks, ulong block_size, bool random);
double write64(FileRef &f, uint64_t addressInfo, ulong no_blocks, ulong block_size, bool random);

ulong my_rand(ulong limit);
uint64_t my_rand64(uint64_t limit);
void fillBuffer(char * buffer, ulong size);
int initialise(int argc, char* argv[]);
void ArgHelp(void);
int Countfiles(void);
void ArgHelp(void);
int Create_files(int number, int size);
void printout(void);
int stop(void);
int scan_param(int c, char* v[], int start_index);
int netSend(const void* lpBuf, int nBufLen);

void openFile(FileRef &f, string filename, bool directio);
void getFileLength64(FileRef &f, uint64_t &length64);
bool openSocket(string SERVER);
void closeSocket();
int getNetworkData(char t[256]);
int setData(const void* lpBuf, int nBufLen);
