/******************************************************************************
 ** Media storage meter
 ** Main program
 *****************************************************************************/
// © BBC 2011	DPB 1/06/2011
// See http://msmeter.sourceforge.net/

#include "meter.h"

using namespace std;

char * buffer;
enum {seq_read, rand_read, seq_write, rand_write};

string ROOT, OUTPUT_FILE, SERVER;

int    FILENUMBER=0;
int    CREATE=0;
ulong  WRITE_FRACTION=10;
ulong  RANDOM_FRACTION=50;
int    OUTFILE=0;
int    DELAY=0;
int    VERBOSE=0;
ulong  MAX_SIZE=1<<20;		//     1,048,576 (1M), part of block 
int    NETWORK=0;
int    FILE_SHARING=0;

int PER_BLOCK = 1; //if times for every block are required
int TIMESTAMP = 1; //if a timestamp is required

//DPB
int   DIRECTIO = 0;
int	   NUMBER_BLOCKTYPE = 7;
int	   BLOCKSIZES[MAXBLOCKSIZETYPE] = {1,2,3,4,5,6,7,0,0,0,0,0};

char output_buffer[150];

FileRef f;
//File64 f64;
fstream outfile,null;

int CREATE_SIZE;

// Arrays for results
double total_time[5][12];
double total_data[5][12];
double average_latency[5][12];
double max_latency[5][12];
int number[5][12];

string mode_str[5] = {"seq_rd","rand_rd","seq_wr","rand_wr","All"};
int socket_open = 0;
int stopped=0;
int started=0;


int main(int argc, char* argv[], char* envp[]) {
  int nRetCode=0;
  srand((unsigned)time(NULL));    // Randomise random number generator

  switch (initialise(argc, argv)){    // Initialise parameters
    case 0 : // Error in intialisation
      cout << "Incorrect initialisation\n\n";
      return nRetCode=1;
    case 1 : // Error in parameters
      cout << "Error in input parameters\n\n";
       return nRetCode=1;
    case 2 : // Help menu
      return nRetCode;
    }
    main_prog();
    return nRetCode;
}


// If using Linux, need an implementation of kbhit.
#if defined (METER_OS_LINUX)
  bool kbhit() {
    fd_set set;
    struct timeval tv;
    struct termios t;

    memset(&tv,0,sizeof(tv));

    tcgetattr(0,&t);
    t.c_lflag&=~ICANON;
    tcsetattr(0,TCSANOW,&t);

    FD_ZERO(&set);
    FD_SET(0,&set);
    select(1,&set,0,0,&tv);
    if (FD_ISSET(0,&set)) {
      getchar();
      return true;
    }
    else return false;
  }
#endif


void main_prog(void){
  int buf_length, any_results = 0;
  TimeStore timer;
  string filename;
  char file[10];
  ulong file_length=0, start_address=0, total_length = 0, blocksize = 0, n_blocks=0;
  int mode, file_select=0, block_size_index=0;
  double t=0;
  bool fdirectio = false;

  //Meter 2, added for 64 bit file addressing
  uint64_t start_address64=0, total_length64=0; 
  uint64_t file_length64 = 0;




  // Clear accumulator memory
  // 4 R/W mode + total x 7 block size + total
  // Meter 2, modified to 4 R/W x 11 block size + total
  for (int i=0;i<5;i++) {
    for (int j=0;j<12;j++) {
      total_time[i][j]=0.0;
      total_data[i][j]=0;
      average_latency[i][j]=0.0;
      max_latency[i][j]=0.0;
      number[i][j]=0;
    }
  }

  // Display helpful information to the user
  cout<<"\nMedia storage meter 2.0.1 BBC R&D 6/6/2011 DPB\n";
  cout<<"----------------------------------------------------\n";
  cout<<"- METER_FILE_UNIX and METER_WIN32 64 bit file access\n";
  cout<<"- Block size and Direct IO selectable from Ctrl App\n";
  cout<<"\nPress any key to terminate program\n\n";

  // Loop ...
  while (!stopped && !stop()) { 
    delay_ms(my_rand(DELAY));
    file_select = my_rand(FILENUMBER);
    sprintf(file,"test%03d",file_select);
    filename=ROOT+file;

	//Meter 2, set Directio Flag (Use Linux NFS only]
	//printf("Direct IO = %d \n", DIRECTIO);
	if (DIRECTIO > 0) {
		fdirectio = true;
	}
	else fdirectio = false;

    // Choose access mode
    // 0=seq read, 1= random read, 2=seq write, 3=random write
    mode = 0;
    if (my_rand(100)<WRITE_FRACTION)  mode=2;  // Set write probability
    if (my_rand(100)<RANDOM_FRACTION) mode+=1; // Set seq/random probability

    // Open and check the test file
    startTimer(timer);
    openFile(f, filename, fdirectio);

    t=stopTimer(timer);

    cout<<file<<"\t";

    if (!f)  {
      cout<<"Error opening file...\n";
      cout<<strerror(errno);
      if (NETWORK) {
        sprintf(output_buffer,"nofiles\n");
        buf_length = strlen(output_buffer);
        netSend(output_buffer, buf_length);
      }
      else stopped=1; 
      continue;
    }

    else {
      if (VERBOSE) {
        sprintf(output_buffer,("%s,"),file);
        buf_length = strlen(output_buffer);
        if (OUTFILE) outfile.write(output_buffer, buf_length);
        if (NETWORK) netSend(output_buffer, buf_length);
      }
    }
	

    // Choose start address and total length
	// modified for 64 bit file addressing
	getFileLength64(f, file_length64);
	

    if (file_length64<MAX_SIZE) total_length64=1024+my_rand64(file_length64-1024);  // 1024 bytes is the minium block size
    else total_length64=1024+my_rand64(MAX_SIZE-1024);
    start_address64 = my_rand64(file_length64 - total_length64);

	//start_address = (ulong)start_address64;
	total_length = (ulong)total_length64;
	//file_length = (ulong)file_length64;

    if (OUTFILE) {
      sprintf(output_buffer,("%llu,"), start_address64);
      buf_length = strlen(output_buffer);
      outfile.write(output_buffer, buf_length);
    }

    // Choose block size
	// Meter 2, allows selection from blocksize range
    //block_size_index=10+my_rand(7);
    //block_size_index=16;  //Fix block size 64K only = 16
	//Block size randomly selected from array of block sizes provided by Ctrl App
	int myrand = int(my_rand((ulong)NUMBER_BLOCKTYPE));
	block_size_index = 9 + BLOCKSIZES[myrand];
	//printf("Number Block Types = %d, Selected Block Size = %d, Block Size Index = %d  \n", NUMBER_BLOCKTYPE, myrand, block_size_index);
	

    // Meter 2 hard commented out modifcation, Ubtunu CIFS cannot do 64K, limit is 65535 (16 bits)
	//if (block_size_index==16) blocksize=65535; // Uncomment to hard code 64K = 65535 bytes
    //else // Uncomment to hard code 64K = 65535 bytes
	blocksize=1<<block_size_index;	// at least 1024 and not greater than 64K
    

    // Adjust the block length to be less than the total data.
    // Meter 2 commeted out mod, to account for 64K=65535, as Linux CIFs max block size for read, 57K for write
	 while (blocksize >= total_length && block_size_index > 10) {
      //if (block_size_index==16) blocksize=65536; // Uncomment to hard code 64K = 65535 bytes
      blocksize /= 2;
      block_size_index--;
    }

    block_size_index -= 10;       // Convert it to an index in the range 0 - 6
    cout<<blocksize/1024<<"k  \t";

    //Verbose mode outputs data to terminal window and sends back to Ctrl App
	if (VERBOSE) {
      sprintf(output_buffer,("%d,%d,%d,"), mode, total_length / 1024, ((blocksize + 1) / 1024));
      buf_length = strlen(output_buffer);
      if (OUTFILE) outfile.write(output_buffer, buf_length);
      if (NETWORK) netSend(output_buffer, buf_length);
    }


    // Allocate memory for buffer
    buffer=new char[blocksize];
    if (!buffer) {
      cout << "Failure to allocate memory block\n\n";
      exit(0);
    }
	//printf("0x%lx \n", (long)buffer);
    // Call the appropriate routine


	double access_time;         // total access time in ms.

	/////
    n_blocks=total_length/blocksize;
    cout<<n_blocks<<"\t";

    if (n_blocks==0) {
      cout << "Error, blocks = 0" << endl;
      cout << "Total length = " << total_length << " Block size = " << blocksize << endl;
      stopped = 1;
      continue;
    }
	
	//declaring the timestamps array, also in the case we're not using it
	//for the moment, the frame size is given by a preprocessor command
	//could be modified to be dynamic
	int frameSize = 20;
	int n_frames = floor((double)n_blocks/frameSize); //acts as a floor function
	
	
	double* timeStamps = new double[n_frames];
	//initializing all values of timeStamps to zero
	//try also with double array[100] = {0}
	for (int erz = 0; erz < n_frames; erz++) timeStamps[erz] = 0;
	

    startTimer(timer);

	
	switch (mode) {
		 case seq_read:
			access_time=read64(f, start_address64, n_blocks, blocksize, false, frameSize, timeStamps);  break;
		  case seq_write:
			access_time=write64(f, start_address64, n_blocks, blocksize, false, frameSize, timeStamps); break;
		  case rand_read:
			access_time=read64(f, file_length64, n_blocks, blocksize, true, frameSize, timeStamps);     break;
		  case rand_write:
			access_time=write64(f, file_length64, n_blocks, blocksize, true, frameSize, timeStamps);    break;
	}
    t+=stopTimer(timer);
    t-=access_time;
    any_results = 1;

  	// De-allocate buffer memory
    if (buffer) delete [] buffer;

    // write read times and rates
	sprintf(output_buffer,("%6.2f MB/s, Latency %5.2f ms  fref=%2d FS=%11llu  TL=%8llu  SA=%11llu\n"),(n_blocks*blocksize/1048576.0)/(access_time/1000.0),t, f, file_length64, total_length64, start_address64);
    cout<<output_buffer;
	   
    if (VERBOSE){
		//printing basic stuff
		sprintf(output_buffer,("%f,%f"),(n_blocks*blocksize/1048576.0)/(access_time/1000.0),t);
		buf_length=strlen(output_buffer);
		if (OUTFILE) outfile.write(output_buffer,buf_length);
		if (NETWORK) netSend(output_buffer,buf_length);
		
		sprintf(output_buffer,("\n"));
		buf_length=strlen(output_buffer);
		if (OUTFILE) outfile.write(output_buffer,buf_length);
		if (NETWORK) netSend(output_buffer,buf_length);
		
		//now we append the timestamp (server time) per read/write, if option selected
		if (TIMESTAMP) {
			string timeStamp = getTimeStampNTP();
			//cout << "DEBUG: meter.cpp sending this : " << timeStamp << endl;
			sprintf(output_buffer,(":%s\n"),timeStamp.c_str());
			buf_length=strlen(output_buffer);
			if (OUTFILE) outfile.write(output_buffer,buf_length);
			if (NETWORK) netSend(output_buffer,buf_length);
		}
		
		//now we append the times per frame, if option selected
		sprintf(output_buffer,(""));
		if (PER_BLOCK) {
			stringstream ss;
			int lineCounter = 0;
			for (int frame = 0; frame < n_frames; frame++) {
				//concatenate string
				ss << ":" << timeStamps[frame];
				
				if ((lineCounter == 10) || (frame == (n_frames-1))) {
					lineCounter = 0;
					ss << "\n";
					sprintf(output_buffer, ("%s"), ss.str().c_str());
					buf_length=strlen(output_buffer);
					if (OUTFILE) outfile.write(output_buffer,buf_length);
					if (NETWORK) netSend(output_buffer,buf_length);
					 
					ss.str("");
				}
				lineCounter++;
			}
			//emptying the stringstream
			ss.str("");
			ss.clear();
		}
    }

    // Accumulate data
    // Individual
    total_time[mode][block_size_index] += access_time;
    total_data[mode][block_size_index] += (double)n_blocks * (double)blocksize;
    // All modes
    total_time[4][block_size_index] += access_time;
    total_data[4][block_size_index] += (double)n_blocks * blocksize;
    // All blocksizes
    total_time[mode][11] += access_time;
    total_data[mode][11] += (double)n_blocks * (double)blocksize;
    // Overall
    total_time[4][11] += access_time;
    total_data[4][11] += (double)n_blocks * (double)blocksize;

    // Individual
    average_latency[mode][block_size_index] += t;
    number[mode][block_size_index]++;
    // All modes
    average_latency[4][block_size_index] += t;
    number[4][block_size_index]++;
    // All blocksizes
    average_latency[mode][11] += t;
    number[mode][11]++;
    // Overall
    average_latency[4][11] += t;
    number[4][11]++;

    // Individual
    double * p = &max_latency[mode][block_size_index];
    if ( t > *p) *p = t;

    // All modes
    p = &max_latency[4][block_size_index];
    if ( t > *p) *p = t;

    // All blocksizes
    p = &max_latency[mode][11];
    if ( t > *p) *p = t;

    // Overall
    p = &max_latency[4][11];
    if ( t > *p) *p = t;

	//deleting the timeStamps array from the memory
	delete [] timeStamps;
  }

  //end of while loop

  if (VERBOSE) {
    if (OUTFILE) outfile.write("\n", 1);
    if (NETWORK) netSend("\n", 1);
  }

  if (any_results) printout();
  if (OUTFILE) outfile.close();
  if (socket_open) {
    netSend("bye\n", 3);
    closeSocket();
    socket_open = 0;
  }
}


/******************************************************************************
 ** Printout results
 *****************************************************************************/
void printout(void) {
  int i, j, buf_length, temp;
  temp = NETWORK;
  NETWORK = 0;    //Switch off network output of averaged results, we may not need them

  /**************** Print out average transfer rates ************************/
  sprintf(output_buffer,"Results summary - average transfer rates, Mbytes/s\n\tBlock size\n");
  buf_length = strlen(output_buffer);
  cout<<output_buffer;
  if (OUTFILE) outfile.write(output_buffer, buf_length);
  if (NETWORK) netSend(output_buffer, buf_length);

  sprintf(output_buffer,"Mode\t    1k\t    2k\t    4k\t    8k\t   16k\t   32k\t   64k\t   128K\t   256K\t   512K\t   1M\t   All\n");
  buf_length = strlen(output_buffer);
  cout<<output_buffer;
  if (OUTFILE) outfile.write(output_buffer, buf_length);
  if (NETWORK) netSend(output_buffer, buf_length);

  for (i=0;i<5;i++) {
    sprintf(output_buffer,"%s\t", mode_str[i].c_str());
    buf_length = strlen(output_buffer);
    cout<<output_buffer;
    if (OUTFILE) outfile.write(output_buffer, buf_length);
    if (NETWORK) netSend(output_buffer, buf_length);

    for (j=0;j<12 ;j++) {
      if(total_data[i][j] == 0.0) sprintf(output_buffer,"%7.2f\t", 0.0);
      else sprintf(output_buffer,"%7.2f\t", total_data[i][j] / total_time[i][j] / 1000.0);

      buf_length = strlen(output_buffer);
      cout<<output_buffer;
      if (OUTFILE) outfile.write(output_buffer, buf_length);
      if (NETWORK) netSend(output_buffer, buf_length);
    }

    cout<<endl;
    if (OUTFILE) outfile.write("\n",1);
    if (NETWORK) netSend("\n",1);
  }

  /**************** Print out average latency ************************/
  sprintf(output_buffer,"\nResults summary - average latency, ms\n\tBlock size\n");
  buf_length=strlen(output_buffer);
  cout<<output_buffer;
  if (OUTFILE) outfile.write(output_buffer, buf_length);
  if (NETWORK) netSend(output_buffer, buf_length);

  sprintf(output_buffer,"Mode\t    1k\t    2k\t    4k\t    8k\t   16k\t   32k\t   64k\t   128K\t   256K\t   512K\t   1M\t   All\n");
  buf_length=strlen(output_buffer);
  cout<<output_buffer;
  if (OUTFILE) outfile.write(output_buffer, buf_length);
  if (NETWORK) netSend(output_buffer, buf_length);

  for (i=0;i<5;i++) {
    sprintf(output_buffer,"%s\t", mode_str[i].c_str());
    buf_length=strlen(output_buffer);
    cout<<output_buffer;
    if (OUTFILE) outfile.write(output_buffer, buf_length);
    if (NETWORK) netSend(output_buffer, buf_length);

    for (j=0;j<12;j++) {
      if(average_latency[i][j]==0.0) sprintf(output_buffer,"%7.1f\t", 0.0);
      else sprintf(output_buffer,"%7.1f\t", average_latency[i][j] / (double) number[i][j]);
      buf_length=strlen(output_buffer);
      cout<<output_buffer;
      if (OUTFILE) outfile.write(output_buffer, buf_length);
      if (NETWORK) netSend(output_buffer, buf_length);
    }
    cout<<endl;
    if (OUTFILE) outfile.write("\n",1);
    if (NETWORK) netSend("\n",1);
  }

  /**************** Print out maximum latency ************************/
  sprintf(output_buffer,"\nResults summary - maximum latency, s\n\tBlock size\n");
  buf_length=strlen(output_buffer);
  cout<<output_buffer;
  if (OUTFILE) outfile.write(output_buffer,buf_length);
  if (NETWORK) netSend(output_buffer,buf_length);

  sprintf(output_buffer,"Mode\t    1k\t    2k\t    4k\t    8k\t   16k\t   32k\t   64k\t   128K\t   256K\t   512K\t   1M\t   All\n");
  buf_length=strlen(output_buffer);
  cout<<output_buffer;
  if (OUTFILE) outfile.write(output_buffer,buf_length);
  if (NETWORK) netSend(output_buffer,buf_length);

  for (i=0;i<5;i++) {
    sprintf(output_buffer,"%s\t", mode_str[i].c_str());
    buf_length=strlen(output_buffer);
    cout<<output_buffer;
    if (OUTFILE) outfile.write(output_buffer,buf_length);
    if (NETWORK) netSend(output_buffer,buf_length);

    for (j=0;j<12;j++) {
      if (max_latency[i][j]==0.0) sprintf(output_buffer,"%7.3f\t", 0.0);
      else sprintf(output_buffer,"%7.3f\t", max_latency[i][j] / 1000.0);
      buf_length=strlen(output_buffer);
      cout<<output_buffer;
      if (OUTFILE) outfile.write(output_buffer,buf_length);
      if (NETWORK) netSend(output_buffer,buf_length);
    }
    cout<<endl;
    if (OUTFILE) outfile.write("\n", 1);
    if (NETWORK) netSend("\n", 1);
  }
  	
  NETWORK=temp;   //Restore network setting
}


/**** This is where the remote control interface is implemented  ****
 **** Function returns 0 to continue program, 1 to terminate     ****
 **** It can change parameters in mid-flow if required           ****/

int stop() {
/*
  In here is the network control stuff.
  If the socket is not open then open it and connect to the server.
  Then wait for the start command.

  If it is open then interpret any commands that come along,
  including 'start', 'stop' or 'pause'.
*/

  static int first_time = 1;
  string str;
  char t[256];
  char * ta[35]; //Meter 2, changed from 20 to 35 for extra control parameters
  int c, rv, loop_flag = 1, l;
  char * p;

  if (NETWORK) {
    if (!socket_open) {
      bool socketOpen=openSocket(SERVER);
      if(socketOpen)socket_open=1;
      else return 1;
    }

    // By this stage we must have an open network connection
    // Wait here for server start response input

    if(first_time) {
      if (!netSend("hello\n",6)) return 1;
      else cout<<"Waiting for server response"<<endl;
    }

    while (loop_flag) {  // key pressed check removed.
      rv=getNetworkData(t);
     //rv=recv(sockfd,t,255,0);
      p = t;

      while (rv > 0 && rv < 255) {
        l = strlen(p);
        //if(rv > strlen(t) + 1) cout << "more than one string " << rv << strlen(t) << endl;
        //if(rv > strlen(t) + 1) t[strlen(t)] = ' ';		// This is a very crude get-around
        // that converts two inputs into one string
        cout << p << endl;
        //Here if a message has come in
        // First do handshake with server
        if (first_time) {
          if(!strncmp(p,"hellotoyou", 10)) {
            first_time = 0;
            cout << "Waiting for server commands" << endl;
          }
          else {
            cout << "Failed to connect to server" << endl;
            return 1;
          }
        }

       //Parse the message, checking for keywords.
       // first tokenise the received string and put into an array of pointers
       // Note - there may be more than one string object in t
        c = 0;
        
		// The max value of c determines the number of tokens that can be used
		// This was increased from 20 to 35, when new parameters were added
		while ((ta[c] = strtok(((c == 0)?(p):(NULL))," ")) && c < 35) {
          //cout << ta[c] << endl;
          c++;
        }
        // now pass array of tokens to interpreter
		// limited by the number tokens (max value of c)
        scan_param(c, ta, 0);

        for (--c;c>=0;c--) {
          if (!strcmp(ta[c],"start") && (Countfiles()>=FILENUMBER)) {
            started = 1;
            cout<<"started\n";
          }
          if (!strcmp(ta[c],"pause")) started = 0;
          if (!strcmp(ta[c],"stop")) return 1;  //If the command includes stop then terminate the application immediately.
        }
        rv-=(l+1);
        p+=(l+1);
      }
      if (started) loop_flag = 0;
      if (kbhit()) return 1; 
    } // This is the end of loop_flag loop
  }
  if (!kbhit()) return 0;
  else return 1;
}

int netSend(const void* lpBuf, int nBufLen){
  if(!setData(lpBuf,nBufLen)) {
    NETWORK = 0;
    socket_open = 0;
    stopped = 1;
    return 0;
  }
  return 1;
}


/******************************************************************************
 ** Initalisation Routines
 *****************************************************************************/

int initialise(int argc, char* argv[]){
  // Scan command line arguments and set flags	
  if (argc>1 && !strcmp(argv[1], "help")) {
    ArgHelp();
    return 2;
  }

  if (argc==1) {
    // Count the number of files and set FILENUMBER
    FILENUMBER=Countfiles();
    if (FILENUMBER==0) {
      cout << "No test files found on default path\n\n";
      return 0;
    }
    else cout << "Number of test files found on default path = " << FILENUMBER << endl;
  }

  switch (scan_param(argc, argv, 1)) {
    case 1 : return 1 ;
    case 2 : return 2 ;
  }
  return 3;
}


int scan_param(int c, char* v[], int start_index) {

  // Process arguments from command line or network control input.
  // start_index specifies whether the array of parameter strings
  // starts at 1 (eg. for command line input) or 0 (for network input)
  // Pre converted in tokens, see @ line 535 in int stop() routine
  // Access to token values is limited by the number tokens (max value of c)
	int  t, t2, cmax;;
	int static output_file_open = 0;
	bool set_file_number_flag = false;
	//CString str;
	//int i = 2;

	c--;      // c now = the highest index in the array of pointers
	cmax=c;   // cmax = highest valid index

	while (c>=start_index) {

		// Meter 2, get list of block sizes
		if (!strcmp(v[c],"-bs")) {
				//t = atoi(v[c + 1]);
				//size_t vSize = sizeof(v);
				//printf ("sizeof V = %d, c=%d\n", vSize, c);
				//printf("t=%d, vc=%s, vc1=%s vc2=%s vc3=%s vc4=%s vc5=%s\n", t, v[c], v[c+1], v[c+2], v[c+3], v[c+4], v[c+5]);
				t = atoi(v[c + 1]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[0] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[0]);
				}
				t = atoi(v[c + 2]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[1] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[1]);
				}
				t = atoi(v[c + 3]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[2] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[2]);
				}
				t = atoi(v[c + 4]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[3] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[3]);
				}
				t = atoi(v[c + 5]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[4] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[4]);
				}
				t = atoi(v[c + 6]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[5] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[5]);
				}
				t = atoi(v[c + 7]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[6] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[6]);
				}
				t = atoi(v[c + 8]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[7] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[7]);
				}
				t = atoi(v[c + 9]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[8] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[8]);
				}
				t = atoi(v[c + 10]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[9] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[9]);
				}
				t = atoi(v[c + 11]);
				if ((t > 1) || (t < MAXBLOCKSIZETYPE)){
					BLOCKSIZES[10] = t;
					//printf("Block Size = %d \n", BLOCKSIZES[10]);
				}
		}		

		// Meter 2, get number of block size types
		if (!strcmp(v[c],"-bn")) {
			t = atoi(v[c + 1]);
			if ((t < 1) || (t > MAXBLOCKSIZETYPE)){
				puts("Maximum number of block sizes limited to range 1 - 11");
				return 1;
			}
			else { 
				NUMBER_BLOCKTYPE=t;
				//printf("Number Block Size Types = %d \n", t);
			}
		}

		// Meter 2, get Direct IO flag
		if (!strcmp(v[c], "-dio")) {
			t = atoi(v[c + 1]);
			if ((t > -1) || (t < 2)) {
				DIRECTIO = t;
				//printf("Direct IO = %d \n", DIRECTIO);
			}
		}

		// Original meter code
		// Do this here saves checking all the others for enough parameters present
		if (!strcmp(v[c], "-v"))  VERBOSE = 1;
		if (!strcmp(v[c], "-v-")) VERBOSE = 0;
		if (!strcmp(v[c], "-f"))  FILE_SHARING = 1;
		if (!strcmp(v[c], "-f-")) FILE_SHARING = 0;

		// all the rest require one more parameter after the flag

		if (c<cmax) {

			// Path name
			if (!strcmp(v[c],"-p")) {
				ROOT = v[c+1];		
				ROOT += "/";
				// if (ROOT.GetAt(ROOT.GetLength() - 1) != '\\') ROOT += "/";
				if (NETWORK && !set_file_number_flag) FILENUMBER = 0;			// reset file number on change of path over network
			}

			// Number of files
			if (!strcmp(v[c],"-n")) {
				t=atoi(v[c+1]);
				if ((t<0)||(t>1000)) {
				puts("number of files limited to range 0 - 1000");
				return 1;
			}
			else {
				FILENUMBER=t;
				set_file_number_flag=true;
			}
		}

		// Write fraction
		if (!strcmp(v[c],"-w")) {
        t=atoi(v[c+1]);
        if ((t<0)||(t>100)) {
			puts("write fraction limited to range 0% - 100%");
			return 1;
        }
			else WRITE_FRACTION=t;
		}

		// Random fraction
		if (!strcmp(v[c],"-r")) {
			t=atoi(v[c+1]);
			if ((t<0)||(t>100)) {
				puts("random fraction limited to range 0% - 100%");
				return 1;
			}
			else RANDOM_FRACTION=t;
		}

		// Maximum total length
		if (!strcmp(v[c],"-l")) { 	// Get maximum total length
			t2=atoi(v[c+1]);
			if ((t2<2)||(t2>2097151)){
				puts("maximum length limited to range 2 - 2,097,151 Kbyte (2 Gbyte)");
				return 1;
			}
			else MAX_SIZE=(ulong)(t2<<10);
		}

		// Create file flag
		if (!strcmp(v[c], "-c")) {  // Get create size (Mbytes)
			t2 = atoi(v[c+1]);
			if ((t2 < 1) || (t2 > 40960)) {
				puts("Create size limited to range 1 - 40960 Mbyte (40Gbyte)");
				return 1;
			}
			else {
				CREATE_SIZE = t2;
				CREATE = 1;
				cout<<"Create file size: "<<CREATE_SIZE<<"MB."<<endl;
			}
		}

		// Maximum delay
		if (!strcmp(v[c],"-d")) {
			t = atoi(v[c + 1]);
			if ((t < 0) || (t > 10000)){
				puts("Maximum delay limited to range 0 - 10000 ms");
				return 1;
			}
			else DELAY=t;
		}

 
		// Output file
		if (!strcmp(v[c],"-o")) {
			OUTFILE=1;
			OUTPUT_FILE=v[c+1];
		}

		// Remote control
		if (!strcmp(v[c],"-rm")) {
			NETWORK=1;
			SERVER=v[c+1];
		}
    }
    c--;

}

  if(CREATE){
    if (FILENUMBER!=0) {
      if (Create_files(FILENUMBER, CREATE_SIZE) == 0) {
        cout << "Failed to create test files.\n\n";
        if (!NETWORK) return 1;
      }
      else CREATE = 0;
    }
    else{
      cout << "Error - create file flag set but no number of files specified.\n";
      if (!NETWORK) return 1;
    }
    if (!NETWORK) return 2;     // stop after creating the files
  }

  else {
    t = Countfiles();           // count the files present
    //cout << "Number of files found = " << t << endl;

    if (FILENUMBER==0) FILENUMBER = t;	// If 0 then use all the files
    if (FILENUMBER>t) {
      cout << "Not enough files found\n\n";
      if (!NETWORK) return 1;
    }
  }

  if (FILENUMBER==0) {
    // if we still have 0 here then something is wrong unless NETWORK is enabled
    if (!NETWORK) {
      cout << "No files found and 'create' function not set.\n";
      return 1;
    }
  }

  // If output file enabled open file.
  if (OUTFILE && !output_file_open) {

    // Open the output file
    outfile.open(OUTPUT_FILE.c_str(),ios::out);  
    if (outfile.good()) {
      //cout<<"Output file "<<OUTPUT_FILE<<" opened\n";
      output_file_open = 1;
      if (VERBOSE) {
        sprintf(output_buffer,("Filename, mode, total length (Kbyte), block size (Kbyte), average data transfer rate (Mbyte/s), latency (ms)\n"));
        outfile.write(output_buffer,strlen(output_buffer));
        outfile.flush();
        cout<<output_buffer;
      }
    }
    else {
      cout<<"Error opening output file...\n";
      return 1;
    }
    // OUTFILE=;
    cout<<"File opened\n";
  }
  return 0; 
}


 // Create files
int Create_files(int number, int size) { // size in Mbytes (2 ^ 20^)
  // returns 1 for success
  int     c, i;
  string  filename;
  char    str[10];
  fstream f;
  char *  buff;
  bool    opened;

  ulong blocksize = 1 << 16;

  if ((number * size) > 200) cout << "This could take a long time." << endl;
  else if ((number * size) > 1000) cout << "This could take a very long time." << endl;

  buff = new char[blocksize];
  if (!buff) {
    cout << "Failure to allocate memory block\n\n";
    exit(0);
  }
  fillBuffer(buff, blocksize);

  c = 0;

  while (c < number) {
    //for (c = 0 ; c < number ; c++) {
    filename = ROOT;
    //str.Format("test%03d", c);
    sprintf(str,"test%03d",c);
    filename += str;
    //cout << filename << endl;

    f.open(filename.c_str(),ios::in | ios::out | ios::trunc);
    opened=f.good();
    //cout<<"Opened: "<<opened<<endl;

    if (!f.is_open()) {
      //cout << "Failed to open file - "<< filename << endl;
      return 0;
    }

    for (i=0;i<size<<4;i++) {     // 1 Mbyte = 16 blocks
      f.write(buff, blocksize);
      cout<<".";
    }
    cout <<  "completed\n";
    f.close();
    c++;
  }
  if (buff) delete [] buff;
  return 1;
}


// Countfiles - returns number of files found; stops at the first one out of sequence
int Countfiles() {
  int     count=0;
  int     end=0;;
  string  filename;
  char    str[10];
  fstream f;

  while (!end) {
    filename=ROOT;
    sprintf(str,("test%03d"),count);
    filename += str;
    f.open(filename.c_str(),ios::in);  // Try and open test file
    if (f.good()) count++;              // File found, continue looking
    else end=1;
    f.close();
  }

  return count;
}


void ArgHelp(void) {
  puts("usage -\tmeter [help] | [[-p <root>] [-n N] [-w W] [-r R] ...");
  puts("\t\t [-l L] [-c S] [-d D] [-o <outfile>] [-v | -v-] [-rm SRV]]\n");
  puts("\t\t [-dio D] [-bn N] [-bs B1 B2 B3B4 B5 B6 B7 B8 B9 B10 B11]\n");
  puts("File server test programme. Accesses a set of test files, reading"); 
  puts("or writing in sequential or random blocks. The filenames, total data");
  puts("lengths, block lengths and breaks between accesses are chosen at random.");
  puts("Filenames are currently restricted to 'test000', 'test001', .., 'test999'.\n");
  puts("Command line parameters-");
  puts("help\tThis help screen.");
  puts("-p\tTest file path. Default = current directory. Final '\\' optional.");
  puts("-n\tN = number of test files to be used. Default = counting existing ones.");
  puts("-w\tPercentage of write access (W = 0 to 100). Default 10%.");
  puts("-r\tPercentage of random access (R = 0 to 100). Default 50%.");
  puts("-l\tMax. length of data transfer, Kbytes. L = 2 - 2,097,151. Default 1024.");
  puts("-c\tCreate files (S = size in Mbytes). -c S must be accompanied by -n N.");
  puts("\tStops after creating the files - not in remote control mode.");
  puts("-d\tMaximum delay between file accesses in ms. D = 0 - 10,000, default 0.");
  puts("-o\tOptional output file for results. Default = no output file.");
  puts("-v\tVerbose file output - output line for every file access (-v- = off).");
  //puts("-f\tFile sharing. Default off-skips over files already in use. (-f- = off).");
  puts("-bn\tNumber of block sizes to use. N = 1 to 11. Default 7");
  puts("-bs\tBlock sizes to use. Bx = 1 to 11. Default 1 2 3 4 5 6 7 0 0 0 0");
  puts("-rm\tEnable remote control. SRV = server name (IP address or machine name).\n");
  puts("All parameters may occur in any order, except 'help', though the modifiers");
  puts("<root>, N, W, R, L, S, D, <outfile> and SRV must immediately follow their");
  puts("flags, -p, -n, -w, -c, -r, -l, -d, -dio, -bn, -bs, -o or -rm.");
  puts("Press any key to abort program.");
}


