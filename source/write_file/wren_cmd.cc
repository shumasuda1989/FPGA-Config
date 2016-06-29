
// wren_cmd

#include "wren_cmd.h"
#include "rbcp_com.h"

using namespace std;

void wren_cmd(const char *ip_addr, unsigned int port)
{
   
   // BufSize   = 512
   // sock_Addr = (ip_addr, port)
   // socket.setdefaulttimeout(2.0)

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

  // tempInt = 0x000000060000;
  // sendData[5]= (char)(0xFF & tempInt);
  // sendData[4]= (char)((0xFF00 & tempInt)>>8);
  // sendData[3]= (char)((0xFF0000 & tempInt)>>16);
  // sendData[2]= (char)((0xFF000000 & tempInt)>>24);
  // sendData[1]= (char)((0xFF00000000 & tempInt)>>32);
  // sendData[0]= (char)((0xFF0000000000 & tempInt)>>40);
  sendData[5]= (char)0;
  sendData[4]= (char)0;
  sendData[3]= (char)(0x060000 >>16);
  sendData[2]= (char)0;
  sendData[1]= (char)0;
  sendData[0]= (char)0;

  int rcvdBytes;
  rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

  if(rcvdBytes<0) exit(EXIT_FAILURE);
   
  if ( (recvData[1] & 0x0F) != 0x08 ) {
    cerr << "Receive Error (wren_cmd)" << endl;
    exit(-1);
  }
   
  sndHeader.id=1;
  sndHeader.length=1;
  sndHeader.address=htonl(0x0000000B); //rbcp address

  sendData[0]= (char)(0xFF & 0);

  rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

  if(rcvdBytes<0) exit(EXIT_FAILURE);
   
  if ( (recvData[1] & 0x0F) != 0x08 ) {
    cerr << "Receive Error (wren_cmd)" << endl;
    exit(-1);
  }

  if ( (recvData[2] & 0x0F) != 0x01 ) {
    cerr << "Receive Error (wren_cmd)" << endl;
    exit(-1);
  }

}   
   
