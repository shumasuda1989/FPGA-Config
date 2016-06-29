
#include "rbcp_com.h"
#include "erase_cmd.h"

#define BufSize 512

using namespace std;

void erase_cmd(const char *ip_addr, unsigned int port)
{

  char sendData[UDP_BUF_SIZE]; 
  char recvData[UDP_BUF_SIZE];
  char dispMode=0;
  //unsigned int tempInt;

  struct rbcp_header sndHeader;
  sndHeader.type=RBCP_VER;
  sndHeader.command= RBCP_CMD_WR;
  sndHeader.id=0;
  sndHeader.length=7;
  sndHeader.address=htonl(0x00000005);

  sendData[0] = (char)0x00;
  sendData[1] = (char)0x00;
  sendData[2] = (char)0x00;
  sendData[3] = (char)0xC7;
  sendData[4] = (char)0x00;
  sendData[5] = (char)0x00;
  sendData[6] = (char)0x00;
   
  int rcvdBytes;
  rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

  if(rcvdBytes<0) exit(EXIT_FAILURE);
   
  if ( (recvData[1] & 0x0F) != 0x08 ) {
    cerr << "Receive Error (write_spi_cmd)" << endl;
    exit(-1);
  }


}
