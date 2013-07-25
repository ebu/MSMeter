

#include "meter.h"

#define SERVERIP "127.0.0.1"
#define PORTNUMBER 123

using namespace std;


#if defined (METER_OS_WIN32)

string getTimeStampNTP(void) {
	
	return "23849184923092";
}


#elif defined (METER_OS_LINUX)

#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

//socket file descriptor
int socketfd;

bool connected = false;

//returns string containing the timestamp
string getTimeStampNTP(void) {
	
	string failed = "no ntp time";
	
	if (!connected) {
		int checkk = connectNTPSocket(SERVERIP);
		cout << "DEBUG: ntp.cpp checkk value for connection of socket : " << checkk << endl;
		if (checkk == -1) return failed;//in case of no connection this is the timestamp
		connected = true;
	}
		//connected
		
		//message to send
		char msgNTP[1];
		msgNTP[0] = 0x23;
		int lenNTP = 48;
		
		//cout << "DEBUG: ntp.cpp before sending" << endl;
		
		send(socketfd, msgNTP, lenNTP, 0);
		
		ssize_t receivedBytesNTP;
		char incomingDataBufferNTP[60];
		
		//cout << "DEBUG: ntp.cpp before receiving" << endl;
		
		receivedBytesNTP = recv(socketfd, incomingDataBufferNTP, 60, 0);
		
		//cout << "DEBUG: ntp.cpp received in bytes: " << receivedBytesNTP << endl;
		
		if (receivedBytesNTP == 0) return failed;	//host shut down
		if (receivedBytesNTP ==-1) return failed;	//error
		
		//parsing the results
		//the received info is 48 bytes, the last 8 bytes (64 bits) are the ones that
		//interest us
		
		//seconds (first 32 bits)
		unsigned int seconds = 0;
		for (unsigned n = 0; n < 4; n++) {
			seconds = (seconds << 8) + incomingDataBufferNTP[ n + 40 ];
		}
		//second fraction (last 32 bits), we then have to convert it in microseconds
		unsigned int second_fraction = 0;
		for (unsigned n = 0; n < 4; n++) {
			second_fraction = (second_fraction << 8) + incomingDataBufferNTP[ n + 44 ];
		}
		int microseconds = (long long) second_fraction * 1000000 / 4294967296;
		
		//cout << "DEBUG: NTP.cpp time : " << seconds << endl;
		
		stringstream ssNTP;
		ssNTP << seconds;
		ssNTP << "s";
		ssNTP << microseconds;
		
		return ssNTP.str();
	


}

//accepts the server name as a string
//opens the socket
//returns -1 if not connected
int connectNTPSocket(string serverIP) {
	struct sockaddr_in dest_addr;
		socketfd=socket(AF_INET,SOCK_DGRAM,0);
		dest_addr.sin_family=AF_INET;
		dest_addr.sin_port=htons(123);
		dest_addr.sin_addr.s_addr=inet_addr(serverIP.c_str());
		memset(&(dest_addr.sin_zero),'\0',8);

		if (connect(socketfd,(sockaddr *)&dest_addr,sizeof(sockaddr))==-1) {
			cout<<"Failed to connect; error number "<<errno<<endl;
			return -1;
		} else {
			cout << "DEBUG: Connected to NTP server.." << endl;
			return 0;
		}
}


#endif
