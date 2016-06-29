
//read_buf_to_file

#include "rbcp_com.h"
#include "read_buf_to_file.h"
#define BUFSIZE 512

using namespace std;

void read_buf_to_file(const char *ip_addr, unsigned int port, unsigned int buf_addr, unsigned int data_length, const char *filename)
{
   
  char sendData[UDP_BUF_SIZE]; 
  char recvData[UDP_BUF_SIZE];
  char dispMode=0;
  //unsigned int tempInt;

  struct rbcp_header sndHeader;
  sndHeader.type=RBCP_VER;
  sndHeader.command= RBCP_CMD_RD;
  sndHeader.id=0;
  sndHeader.length=data_length;
  sndHeader.address=htonl(buf_addr);

  int rcvdBytes;
  rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

  if(rcvdBytes<0) exit(EXIT_FAILURE);
   
  if ( (recvData[1] & 0x0F) != 0x08 ) {
    cerr << "Receive Error (read_buf_to_file)" << endl;
    exit(-1);
  }

  ofstream file(filename,ios::out|ios::binary|ios::app);

  for(int j=8;j<8+recvData[3];j++)
    file.write(&recvData[j],1);

  file.close();

}
