/******************************************************************************
 ** Media storage meter
 ** Timers
 *****************************************************************************/

#include "meter.h"
using namespace std;

// Delay until next real-time second rollover after t seconds
void delay(int t) {
  time_t start, finish;
  time (&start);
  while (difftime(time(&finish),start )<t);
}


// Delay for number of ms
void delay_ms(int ms) {
  #if defined (METER_OS_WIN32)
    ULONGLONG l_scale, start, now;
    LARGE_INTEGER lint;
    int end = 0;
    if (ms==0) return;

    if (!QueryPerformanceFrequency(&lint)) cout<<"No performance timer available"<<endl;
    else l_scale=(ULONGLONG)lint.QuadPart/1000;  //results in ms from now on

    QueryPerformanceCounter(&lint);
    start = ((ULONGLONG)lint.HighPart << 32) + (ULONGLONG)lint.LowPart;
    while (!end) {
      QueryPerformanceCounter(&lint);
      now = ((ULONGLONG)lint.HighPart << 32) + (ULONGLONG)lint.LowPart;
      if (((now - start)/l_scale) >= ms ) end = 1;
    }

  #elif defined (METER_OS_LINUX)
    timeval start, now;
    double startdub, nowdub;
    int end=0;
    if(ms==0) return;
    gettimeofday(&start,NULL);
    startdub=start.tv_sec+(start.tv_usec/1000000.0); // start time in seconds
    while (!end) {
      gettimeofday(&now,NULL);
      nowdub=now.tv_sec+(now.tv_usec/1000000.0);     // current time in seconds
      if (((nowdub-startdub)*1000.0)>=ms) end=1;     // *1000 to convert to ms
    }

  #endif
  return;
}

// Sets returnTime to the counter start value
void startTimer(TimeStore &returnTime) { 
  #if defined (METER_OS_WIN32)
    LARGE_INTEGER qpc;
    QueryPerformanceCounter(&qpc);
    returnTime.upper=qpc.HighPart;
    returnTime.lower=qpc.LowPart;

  #elif defined (METER_OS_LINUX)
    timeval time;
    gettimeofday(&time,NULL);
    returnTime.upper=time.tv_sec;
    returnTime.lower=time.tv_usec;

  #endif
  return;
}


// Takes the parameter start and returns with the interval in ms
double stopTimer(TimeStore &startTime) { 

  #if defined (METER_OS_WIN32)
	LARGE_INTEGER lint;
    ULONGLONG l_scale, start, stop;
    if (!QueryPerformanceFrequency(&lint)) cout<<"No performance timer available"<<endl;
    else l_scale=(ULONGLONG)lint.QuadPart/1000;  //results in ms from now on
    QueryPerformanceCounter(&lint);
    stop  = (ULONGLONG) lint.QuadPart;
    start = (((ULONGLONG)startTime.upper) <<32) + (ULONGLONG)startTime.lower;
    return (double)((signed __int64)(stop - start))/(double)((signed __int64)l_scale);
    //return (double)(stop - start_value)/(double)l_scale;

  #elif defined (METER_OS_LINUX)
    timeval stopTime;
    double startdub, stopdub;
    gettimeofday(&stopTime,NULL);
    if (stopTime.tv_sec==startTime.upper) {  // if seconds are the same, just work with usecs
      return (stopTime.tv_usec-startTime.lower)/1000.0; // /1000 to convert from us to ms
    }
    else {
      startdub=startTime.upper+startTime.lower/1000000.0;   // start time is seconds
      stopdub=stopTime.tv_sec+(stopTime.tv_usec/1000000.0); // stop time in seconds
      return (stopdub-startdub)*1000.0;                    // *1000 to convert from s to ms
    }
  #endif
}


// Returns an unsigned long random number between 0 and 'limit'
// 0xff = 255 (8)
// 0xffff = 65,535 64K (16)
// 0xffffff = 16,777,215 16M (24)
// 
unsigned long my_rand(unsigned long limit) {
  unsigned long temp;
  if (limit==0) return 0;

  temp=rand()&0xff;	//max 255 (8)
  if (limit>0xff) {
    temp<<=8;
    temp+=rand() &0xff;	//max 64K (16)
  }
  if (limit>0xffff) {
    temp<<=8;
    temp+=rand()&0xff; //max 16M (24)
  }
  if (limit>0xffffff) {
    temp<<=8;
    temp+=rand()&0xff; //max 4G (32)
  }
  temp%=limit;
  return temp;
}


// Returns an unsigned long long (uint64_t) random number between 0 and 'limit'

uint64_t my_rand64(uint64_t limit) {
  uint64_t temp=0;

  if (limit==0) return 0;

  temp=rand()&0xff;	//max 255 (2^8-1)
  if (limit>0xff) {
    temp<<=(uint64_t)8;
    temp+=(uint64_t)rand() &0xff;	//max 64K (2^16-1)
  }
  if (limit>0xffff) {
    temp<<=(uint64_t)8;
    temp+=(uint64_t)rand()&0xff; //max 16M (2^24-1)
  }
  if (limit>0xffffff) {
    temp<<=(uint64_t)8;
    temp+=(uint64_t)rand()&0xff; //max 4G (2^32-1) = 4,294,967,295
  }
  if (limit>0xffffffff) {
    temp<<=(uint64_t)8;
    temp+=(uint64_t)rand()&0xff; //max 1T (2^40-1) = 1099,511,627,775
  }
  if (limit>0xffffffffff) {
    temp<<=(uint64_t)8;
    temp+=(uint64_t)rand()&0xff; //max 256T (2^48-1) = 281,474,976,710,655
  }
  if (limit>0xffffffffffff) {
    temp<<=(uint64_t)8;
    temp+=(uint64_t)rand()&0xff; //max 52P (2^52-1) = 72,075,594,037,927,935
  }
  if (limit>0xffffffffffffff) {
    temp<<=(uint64_t)8;
    temp+=(uint64_t)rand()&0xff; //max 16E (2^64-1) = 18,446,744,073,709,551,615
  }
  temp%=limit;
  return temp;
}



