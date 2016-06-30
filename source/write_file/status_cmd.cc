
// status_cmd

#include "rbcp_com.h"
#include "status_cmd.h"
#define    BufSize  512

using namespace std;

void status_cmd(const char *ip_addr, unsigned int port) 
{

  char sendData[UDP_BUF_SIZE]; 
  char recvData[UDP_BUF_SIZE];
  char dispMode=0;
  //unsigned int tempInt;

  struct rbcp_header sndHeader;
  sndHeader.type=RBCP_VER;
  sndHeader.command= RBCP_CMD_WR;
  sndHeader.id=0;
  sndHeader.length=6;
  sndHeader.address=htonl(0x00000005); //rbcp address
   
  sendData[0]=(char)0x00;
  sendData[1]=(char)0x00;
  sendData[2]=(char)0x02;
  sendData[3]=(char)0x05;
  sendData[4]=(char)0x00;
  sendData[5]=(char)0x00;
   
  int rcvdBytes;
  rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

  if(rcvdBytes<0){
    cerr << "Receive Error (status_cmd)" << endl;
    exit(EXIT_FAILURE);
  }
   
  // if ( (recvData[1] & 0x0F) != 0x08 ) {
  //   cerr << "Receive Error (status_cmd)" << endl;
  //   exit(-1);
  // }

  sndHeader.command= RBCP_CMD_WR;
  sndHeader.id=1;
  sndHeader.length=1;
  sndHeader.address=htonl(0x0000000B); //rbcp address
  
  sendData[0]=(char)0x00;
   
  rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

  if(rcvdBytes<0){
    cerr << "Receive Error (status_cmd)" << endl;
    exit(EXIT_FAILURE);
  }

  // if ( (recvData[1] & 0x0F) != 0x08 ) {
  //   cerr << "Receive Error (status_cmd)" << endl;
  //   exit(-1);
  // }

  if ( (recvData[2] & 0xFF) != 0x01 ) {
    cerr << "Receive Error (status_cmd)" << endl;
    exit(-1);
  }

}   
   
#ifdef TEST_STATUS_CMD
int main(int argc,char **argv)
{
  if (argc != 3) {
    cerr << "Usage: "<< argv[0] << " <IP> <port>" <<endl;
    return 1;
  }
  status_cmd(argv[1], atoi(argv[2]));

  return 0;

}

#endif
