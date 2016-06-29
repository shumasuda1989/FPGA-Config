
// write_spi_cmd

#include "write_spi_cmd.h"
#include "rbcp_com.h"
#define  BUFSIZE  512

using namespace std;

void write_spi_cmd(const char *ip_addr, unsigned int port, unsigned int spi_reg_addr, unsigned int data_length)
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

  sendData[0]= (char)0x83;

  sendData[2]= (char)(0xFF & data_length);
  sendData[1]= (char)((0xFF00 & data_length)>>8);
   
  sendData[3]= (char)0x02;

  sendData[6]= (char)(0xFF & spi_reg_addr);
  sendData[5]= (char)((0xFF00 & spi_reg_addr)>>8);
  sendData[4]= (char)((0xFF0000 & spi_reg_addr)>>16);
   
  int rcvdBytes;
  rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, sendData, recvData, dispMode);

  if(rcvdBytes<0) exit(EXIT_FAILURE);
   
  if ( (recvData[1] & 0x0F) != 0x08 ) {
    cerr << "Receive Error (write_spi_cmd)" << endl;
    exit(-1);
  }

}

#ifdef TEST_WRITE_SPI_CMD
int main(int argc,char **argv)
{
  if (argc != 5) {
    cerr << "Usage: "<< argv[0] << " <IP> <port> <spi_reg_addr> <data_length>" <<endl;
    return 1;
  }
  write_spi_cmd(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

  return 0;

}

#endif
