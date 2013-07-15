/******************************************************************************
 ** Media storage meter
 ** File access
 *****************************************************************************/
// Multiple versions of read64, write64 and other subroutines.
// Ensure correct flags are set in makefile or visual stuido config.
// 64 bit addressing supported by METER_FILE_UNIX and METER_FILE_WIN32
// 64 bit may not fully supported on some versions, for 32 OS, see comments

#include "meter.h"

using namespace std;

extern char * buffer;


#if defined (METER_FILE_CLIB)
  // ** Standard C Library ** //
  // Supports 64 bit, but not preferred soluiton


  void openFile(FileRef &f, string filename, bool directio) {
    f=fopen(filename.c_str(),"r+b");
    if(f==NULL) cout<<"Error opening file!\n";
  }

  

  void getFileLength64(FileRef &f, uint64_t &length64) {
    fseeko(f,0,SEEK_END);
    length64= (uint64_t)ftello(f);
	}



  double read64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random) {
    TimeStore startTime;
    double  sum=0.0;
    cout<<"CLib read\t";

    fillBuffer(buffer,blockSize);

    if (random) {
      for (int block=0;block<numberBlocks;block++) {
        fseeko(f,my_rand64(addressInfo-(uint64_t)blockSize),SEEK_SET);
        startTimer(startTime);
        if (fread(buffer,1,blockSize,f)==0) cout<<"error";
        fflush(f);
        sum+=stopTimer(startTime);
      }
    }
    else {
      fseeko(f,(off_t)addressInfo,SEEK_SET);
      for (int block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        if (fread(buffer,1,blockSize,f)==0) cout<<"error";
        fflush(f);
        sum+=stopTimer(startTime);
      }
    }
    fclose(f);
    return sum;
  }

  double write64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random)  {
    TimeStore startTime;
    double  sum=0.0;
    cout<<"CLib write\t";

    fillBuffer(buffer,blockSize);

    if (random) {
      for (int block=0;block<numberBlocks;block++) {
        fseeko(f, (off_t)my_rand64(addressInfo-(uint64_t)blockSize),SEEK_SET);
        startTimer(startTime);
        if (fwrite(buffer,1,blockSize,f)==0) cout<<"error";
        fflush(f);
        sum+=stopTimer(startTime);
      }
    }
    else {
      fseeko(f,(off_t)addressInfo,SEEK_SET);
      for (int block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        if (fwrite(buffer,1,blockSize,f)==0) cout<<"error";
        fflush(f);
        sum+=stopTimer(startTime);
      }
    }
    fclose(f);
    return sum;
  }

#elif defined (METER_FILE_UNIX)
// ** Unix System calls ** //
// 64 bit supported, preffered solution
// Do not use Directio IO if SMB/CIFS mount


  void openFile(FileRef &f, string filename, bool directio) {

	// O_DIRECT breaks SMB/CIFS file size reading
	// O_LARGEFILE flag important when opening the file as dermines large file support, 
	// despite use of falgs in make file: -D_FILE_OFFSET_BITS=64 \ -D_LARGEFILE_SOURCE \-D_LARGEFILE64_SOURCE
	
	if (directio) {    
		f=open(filename.c_str(),O_RDWR | O_LARGEFILE | O_DIRECT, 0);
	}
	else {
		f=open(filename.c_str(),O_RDWR | O_LARGEFILE, 0);
	}
  }

  void getFileLength64(FileRef &f, uint64_t &length64) {
      length64= (uint64_t)lseek(f,0,2);
	}

  double read64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random, double * timeStamps) {
    TimeStore startTime;
    double  sum=0.0;
    cout<<"Unix read\t";

    fillBuffer(buffer,blockSize);

    if (random) {
      for (int block=0;block<numberBlocks;block++) {
        lseek(f,(off64_t)my_rand64(addressInfo-(uint64_t)blockSize),0);
        startTimer(startTime);
        read(f,buffer,blockSize);
        double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;
      }
    }
    else {
      lseek(f,(off64_t)addressInfo,0);
      for (int block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        read(f,buffer,blockSize);
        double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;
      }
    }
    close(f);
    return sum;
  }

  double write64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random, double * timeStamps) {
    TimeStore startTime;
    double  sum=0.0;
    cout<<"Unix write\t";

    fillBuffer(buffer,blockSize);

    if (random) {
      for (int block=0;block<numberBlocks;block++) {
        lseek(f,(off64_t)my_rand64(addressInfo-(uint64_t)blockSize),0);
        startTimer(startTime);
        write(f,buffer,blockSize);
        sync();
        double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;
      }
    }
    else {
      lseek(f,(off64_t)addressInfo,0);
      for (int block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        write(f,buffer,blockSize);
        sync();
        double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;
      }
    }
    startTimer(startTime);
    close(f);
    sum+=stopTimer(startTime);
    return sum;
  }



#elif defined (METER_FILE_MFC)
// ** Win32 CFile ** //
// Not applicable for 32 bit OS, e.g. Win 7 32 bit or Win XP 32 bit
// Returns length of 0 for > 4GB files

  void openFile(FileRef &f, string filename, bool directio) {
    CFileException e;
    f.Open(filename.c_str(), CFile::modeReadWrite, &e);
  }

  void getFileLength64(FileRef &f, uint64_t &length64) {
    length64 = f.GetLength();
	//CFileStatus status;
	//f.GetStatus(status);
	//length64 = status.m_size;
	//printf("File length = %llu \n", length64);
  }

 
  double read64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random) { 
    TimeStore startTime;
    double sum=0.0;
    cout<<"MFC read\t";

    fillBuffer(buffer,blockSize);

  
    if (random) {
      for (ulong block=0;block<numberBlocks;block++) {
        f.Seek((off64_t)my_rand64(addressInfo-(uint64_t)blockSize),CFile::begin);
        startTimer(startTime);
        f.Read(buffer,blockSize);
        sum+=stopTimer(startTime);
      }
    }
    else {
      f.Seek((off64_t)addressInfo,CFile::begin);

      for (ulong block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        f.Read(buffer,blockSize);
        //f.Flush();
        sum+=stopTimer(startTime);
      }
    }
    f.Close();
    return sum;
  }

  double write64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random) {
    TimeStore startTime;
    double sum=0.0;
    cout<<"MFC write\t";

    fillBuffer(buffer,blockSize);

    if (random) {
      for (ulong block=0;block<numberBlocks;block++) {
        f.Seek((off64_t)my_rand64(addressInfo-(uint64_t)blockSize),CFile::begin);
        startTimer(startTime);
        f.Write(buffer,blockSize);
        f.Flush();
        sum+=stopTimer(startTime);
      }
    }
    else {
      f.Seek((off64_t)addressInfo,CFile::begin);
      for (ulong block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        f.Write(buffer,blockSize);
        f.Flush();
        sum+=stopTimer(startTime);
      }
    }
    f.Close();
    return sum;
  }



#elif defined (METER_FILE_WIN32)
// ** Win32 API ** //
// Supports 64 bit file addressing

  void openFile(FileRef &f, string filename, bool directio) {
    f=CreateFile(filename.c_str(),GENERIC_READ|GENERIC_WRITE, NULL,
          NULL,OPEN_EXISTING,FILE_FLAG_WRITE_THROUGH,NULL);
    if (f==INVALID_HANDLE_VALUE) printf("Couldn't open file (error %d)\n",GetLastError());
  }


  void getFileLength64(FileRef &f, uint64_t &length64) {
	  LARGE_INTEGER size;
	  GetFileSizeEx(f, &size);
	  length64= (uint64_t)size.QuadPart;
	  //printf("File Length  %llu size = %ld\n", length64, size);
  }

  //version with essentially code testing. Takes timestamps by frame or by block, does data crunching
  //not called
  double read64_old(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random) { 
    TimeStore startTime;
    double sum=0.0;
    cout<<"Win32 read\t";

    fillBuffer(buffer,blockSize);


    ulong bytesRead;
	long addr_low=0;
	long addr_high=0;
	uint64_t addr64=0;

	//setting up a temporary value to store highest timer value for each block read
	//not used yet
	double highestBlockTime = 0.0;
	//setting up an array with speeds for every frame (a frame is consisting of 1 or more blocks), to be defined
	//not used yet
	int blocksPerFrame = 4;
	int numberFrames = numberBlocks / blocksPerFrame;//floor, can be zero if too few frames
	double* timesArray = new double [numberFrames];

	//setting up an array with speeds for every block transmitted
	double* timeStamps = new double [numberBlocks];


	//setting a value for the highest time for every frame read
	//not used yet
	double highestFrameTime = 0.0;


	int frameCounter = 0;
	double frameTime = 0.0;

    if (random) {
      for (ulong block=0;block<numberBlocks;block++) {
		addr64 = my_rand64(addressInfo-(uint64_t)blockSize);
		addr_low = (long)addr64;
		addr_high =(long)(addr64 >> 32); 
        SetFilePointer(f, addr_low, &addr_high, FILE_BEGIN);
        startTimer(startTime);
        if (!ReadFile(f,buffer,blockSize,&bytesRead,NULL)) cout<<GetLastError();
        FlushFileBuffers(f);
		double tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;
		frameTime+=tempTimerValue;
		/*
		if (tempTimerValue > highestBlockTime) {
			highestBlockTime = tempTimerValue;
		}
		
		//filling array
		if ((frameCounter < numberFrames) && ((block % blocksPerFrame) == (blocksPerFrame - 1))) {
			timesArray[frameCounter] = frameTime;
			if (frameTime > highestFrameTime) {
				highestFrameTime = frameTime;
			}
			frameTime = 0.0;
			frameCounter++;
		}
		*/

		//filling timestamps for every block
		timeStamps[block] = tempTimerValue;
      }
    }
    else {
	  addr_low = (long)addressInfo;
	  addr_high =(long)(addressInfo >> 32); 
	  SetFilePointer(f, addr_low, &addr_high, FILE_BEGIN);
      for (ulong block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        if (!ReadFile(f,buffer,blockSize,&bytesRead,NULL)) cout<<GetLastError();
        FlushFileBuffers(f);
        double tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;
		frameTime+=tempTimerValue;

		/*
		if (tempTimerValue > highestBlockTime) {
			highestBlockTime = tempTimerValue;
		}
		
		//filling array
		if ((frameCounter < numberFrames) && ((block % blocksPerFrame) == (blocksPerFrame - 1))) {
			timesArray[frameCounter] = frameTime;
			if (frameTime > highestFrameTime) {
				highestFrameTime = frameTime;
			}
			frameTime = 0.0;
			frameCounter++;
		}
		*/

		//filling timestamps for every block
		timeStamps[block] = tempTimerValue;
      }
    }
    CloseHandle(f);

	//Temporary solution: printing in the clients terminal the lowest block and frame transfer rate
	//and the average block read time
	double averageBlockTime = sum / numberBlocks;
	double averageSpeed = blockSize * numberBlocks * 1000 / (sum * 1048576);
	double worstSpeedForBlock = blockSize * 1000 / (highestBlockTime * 1048576);
	cout << endl;
	cout << "\t Lowest speed for block = " << worstSpeedForBlock << " MB/s with average " << averageSpeed << " MB/s." << endl;

	//printing and checking worst frame transmission
	double worstSpeedForFrame = (blockSize * blocksPerFrame * 1000) / (highestFrameTime * 1048576);
	cout << "\t Lowest speed for frame = " << worstSpeedForFrame << " MB/s" << endl;
	
	/*
	//printing array with times for each frame 
	for (int i=0; i<numberFrames; i++) {
		cout << timesArray[i] << " ";
	}
	cout << endl;
	*/
	
	//printTimeArray(timesArray, numberFrames, blocksPerFrame, blockSize);
	
	//deletion of array structure
	delete [] timesArray;

    return sum;
  }

  double read64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random, double * timeStamps) { 
    TimeStore startTime;
    double sum=0.0;
    cout<<"Win32 read\t";

    fillBuffer(buffer,blockSize);

    ulong bytesRead;
	long addr_low=0;
	long addr_high=0;
	uint64_t addr64=0;

    if (random) {
      for (ulong block=0;block<numberBlocks;block++) {
		addr64 = my_rand64(addressInfo-(uint64_t)blockSize);
		addr_low = (long)addr64;
		addr_high =(long)(addr64 >> 32); 
        SetFilePointer(f, addr_low, &addr_high, FILE_BEGIN);
        startTimer(startTime);
        if (!ReadFile(f,buffer,blockSize,&bytesRead,NULL)) cout<<GetLastError();
        FlushFileBuffers(f);
        double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;
      }
    }
    else {
	  addr_low = (long)addressInfo;
	  addr_high =(long)(addressInfo >> 32); 
	  SetFilePointer(f, addr_low, &addr_high, FILE_BEGIN);
      for (ulong block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        if (!ReadFile(f,buffer,blockSize,&bytesRead,NULL)) cout<<GetLastError();
        FlushFileBuffers(f);
        double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;
      }
    }
    CloseHandle(f);

	//for debuggin purpose, prints array
	printDoubleArray(timeStamps, numberBlocks);
    return sum;
  }


	double write64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random, double * timeStamps) {
    TimeStore startTime;
    double sum=0.0;
    cout<<"Win32 write\t";

    fillBuffer(buffer,blockSize);

    ulong bytesWritten;
	long addr_low=0;
	long addr_high=0;
	uint64_t addr64=0;

    if (random) {
      for(ulong block=0;block<numberBlocks;block++){
		addr64 = my_rand64(addressInfo-(uint64_t)blockSize);
		addr_low = (long)addr64;
		addr_high =(long)(addr64 >> 32); 
        SetFilePointer(f, addr_low, &addr_high, FILE_BEGIN);
        startTimer(startTime);
        if(!WriteFile(f,buffer,blockSize,&bytesWritten,NULL)) cout<<GetLastError();
        FlushFileBuffers(f);
		double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;

      }
    }
    else {
	  addr_low = (long)addressInfo;
	  addr_high =(long)(addressInfo >> 32); 
      SetFilePointer(f, addr_low, &addr_high, FILE_BEGIN);
      for (ulong block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        if(!WriteFile(f,buffer,blockSize,&bytesWritten,NULL)) cout<<GetLastError();
        FlushFileBuffers(f);
        double tempTimerValue = 0.0;
		tempTimerValue = stopTimer(startTime);
        sum+=tempTimerValue;

		//fills the timeStamps array
		timeStamps[block] = tempTimerValue;
      }
    }
    CloseHandle(f);

	//for debugging purpose, temporary
	printDoubleArray(timeStamps, numberBlocks);

    return sum;
  }





#elif defined (METER_FILE_FSTREAM)
// ** C++ fstream ** //
// Not applicable for 32 bit OS, e.g. Win 7 32 bit or Win XP 32 bit
// Returns length of 0 for > 4GB files

  void openFile(FileRef &f, string filename, bool directio) {
    f.open(filename.c_str(),ios::in|ios::out|ios::binary); 
  }

  void getFileLength64(FileRef &f, uint64_t &length64) {
    f.seekg(0,ios::end);
	length64= (uint64_t)f.tellg();
	printf("File length = %u \n", length64);
  }


  double read64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random) { 
    TimeStore startTime;
    double sum=0.0;
    cout<<"C++ read\t";

    fillBuffer(buffer,blockSize);

    if (random) {
      for (ulong block=0;block<numberBlocks;block++) {
	    f.seekg((streamoff)my_rand64(addressInfo-(uint64_t)blockSize),ios::beg);
        startTimer(startTime);
        f.read(buffer,blockSize);
        f.flush();
        sum+=stopTimer(startTime);
      }
    }
    else {
      f.seekg((streamoff)addressInfo,ios::beg);
      for (ulong block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        f.read(buffer,blockSize);
        f.flush();
        sum+=stopTimer(startTime);
      }
    }
    f.close();
    return sum;
  }

  double write64(FileRef &f, uint64_t addressInfo, ulong numberBlocks, ulong blockSize, bool random) {
    TimeStore startTime;
    double sum=0.0;
    cout<<"C++ write\t";

    fillBuffer(buffer,blockSize);

    if (random) {
      for (ulong block=0;block<numberBlocks;block++) {
 	    f.seekg((streamoff)my_rand64(addressInfo-(uint64_t)blockSize),ios::beg);
        startTimer(startTime);
        f.write(buffer,blockSize);
        f.flush();
        sum+=stopTimer(startTime);
      }
    }
    else {
      f.seekg((streamoff)addressInfo,ios::beg);
      for (ulong block=0;block<numberBlocks;block++) {
        startTimer(startTime);
        f.write(buffer,blockSize);
        f.flush();
        sum+=stopTimer(startTime);
      }
    }
    f.close();
    return sum;
  }

#endif


// Fill buffer
void fillBuffer(char * buffer, ulong bufferSize) {
  for (ulong l=0;l<bufferSize;l++) buffer[l] = my_rand(0xff);
}

//uses an array of times to calculate some basic stats and prints them on screen
//function used for testing purposes only
void printTimeArray(double * times, ulong timesSize, int blocksInFrame, int blockSize) {
	//printing the array
	cout << "***Time per frame in ms***" << endl;
	for (ulong i=0; i < timesSize; i++) {
		cout << times[i] << " ";
	}
	cout << endl;


	//printing the speeds of every frame in MB/s
	cout << "***Speed per frame in MB/s***" << endl;
	for (ulong i=0; i < timesSize; i++) {
		cout << (blocksInFrame*blockSize*1000)/(times[i]*1048576) << " ";//weird data, check
	}
	cout << endl;

	//computing total time
	double totalTime = 0.0;
	for (ulong i=0; i < timesSize; i++) {
		totalTime += times[i];
	}
	//and average speed
	cout << "average speed computed with frames : " << (blocksInFrame*blockSize*timesSize*1000)/(totalTime*1048576) << endl;//correct

	//calculating the average value of the speed
	double sum = 0.0;
	double average = 0.0;
	for (ulong i=0; i < timesSize; i++) {
		sum += (blocksInFrame*blockSize*1000)/(times[i]*1048576);
	}
	average = sum/timesSize;

	//calculating standart deviation
	double devSum;
	for (ulong i=0; i < timesSize; i++) {
		double diff = abs(((blocksInFrame*blockSize*1000)/(times[i]*1048576)) - average);
		devSum += (diff*diff);
	}
	
	cout << "Average frame t. : " << average << " MB/s, standart deviation : " << sqrt(devSum/timesSize)  << endl;


	cout << "--------------------------" << endl;
}

void printDoubleArray(double * array, ulong arraySize) {
	for (int i=0; i < arraySize; i++) cout<<array[i]<<" ";
	cout << endl;
}

