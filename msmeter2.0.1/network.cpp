/******************************************************************************
 ** Media storage meter
 ** Network access
 *****************************************************************************/

#include "meter.h"
using namespace std;

#define PORT 50100

#if defined (METER_OS_WIN32)

  #define VC_EXTRALEAN   // Exclude rarely-used stuff from Win headers


  CAsyncSocket Csock;

  bool openSocket(string SERVER) {
    int last_error;
    AfxWinInit(::GetModuleHandle(NULL), NULL,::GetCommandLine(), 0);
    AfxSocketInit();

    Csock.Create();
    Csock.Connect(SERVER.c_str(),PORT);
    while ((last_error = Csock.GetLastError()) == WSAEWOULDBLOCK) {
      Csock.Connect(SERVER.c_str(),PORT);
    }
    if (!last_error){
      cout<<"Failed to connect";
      return false;
    }
    cout << "Opened connection OK" << endl;
    return true;
  }

  int getNetworkData(char t[256]) {
    return Csock.Receive(t, 255);
  }

  void closeSocket() {
    Csock.Close();
  }

  int setData(const void* lpBuf, int nBufLen) {
    int t;
    if (Csock.Send(lpBuf, nBufLen + 1) == -1){
      t=Csock.GetLastError();
      if (t==WSAENOTCONN)
      cout<<"No active server found\n";
      if (t==WSAECONNABORTED)
        cout << "\nConnection aborted by remote server\n";
      return 0;
    }
    else return 1;
  }

#elif defined (METER_OS_LINUX)

  #include <sys/socket.h>
  #include <arpa/inet.h>

  int sockfd;  // Socket file descriptor for network comms

  bool openSocket(string SERVER) {
    struct sockaddr_in dest_addr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(PORT);
    dest_addr.sin_addr.s_addr=inet_addr(SERVER.c_str());
    memset(&(dest_addr.sin_zero),'\0',8);

    if (connect(sockfd,(sockaddr *)&dest_addr,sizeof(sockaddr))==-1) {
      cout<<"Failed to connect; error number "<<errno<<endl;
      return false;
    }
    else {
      int flags=fcntl(sockfd,F_GETFL);
      fcntl(sockfd, F_SETFL,flags|O_NONBLOCK);  // Make connection non blocking
      cout<<"Opened connection OK"<<endl;
      return true ;
    }
  }

  int getNetworkData(char t[256]) {
    return recv(sockfd,t,255,0);
  }

  void closeSocket() {
    shutdown(sockfd,2);
  }

  int setData(const void* lpBuf, int nBufLen) {
    send(sockfd,lpBuf, nBufLen + 1,0);
    return 1;
  }

#endif
