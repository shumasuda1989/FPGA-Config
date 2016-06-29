
// reboot_start
#include "rbcp_com.h"
#include "reboot_start.h"

using namespace std;

void reboot_start(const char *ip_addr, unsigned int port)
{

  char sendData[UDP_BUF_SIZE]; 
  char recvData[UDP_BUF_SIZE];
  char dispMode=0;
  //unsigned int tempInt;

  struct rbcp_header sndHeader;
  sndHeader.type=RBCP_VER;
  sndHeader.command= RBCP_CMD_WR;
  sndHeader.id=0;
  sndHeader.length=1;
  sndHeader.address=htonl(0x0000001F); //rbcp address

  sendData[0] =(char)0xFF;

  rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

}