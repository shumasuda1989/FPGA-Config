
// read_buf_cmd_not_print

#include "rbcp_com.h"
#include "read_buf_cmd.h"
#define   BUFSIZE  512

using namespace std;

void read_buf_cmd(const char *ip_addr, unsigned int port, char *status_list, bool showprint, unsigned int buf_addr, unsigned int data_length)
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

  if(rcvdBytes<0){
    cerr << "Receive Error (read_buf_cmd)" << endl;
    exit(EXIT_FAILURE);
  }

  // if ( (recvData[1] & 0x0F) != 0x08 ) {
  //   cerr << "Receive Error (read_buf_cmd)" << endl;
  //   exit(-1);
  // }

  if( showprint )
    for(int j=8; j<8+recvData[3]; j++)
      {
	if (j % 8 == 7) 
	  printf("0x%.2x\n",(unsigned char)recvData[j]);
	else 
	  printf("0x%.2x ",(unsigned char)recvData[j]);
      }

  status_list[0]=recvData[8];
  status_list[1]=recvData[9];

}


#ifdef TEST_READ_BUF_CMD
int main(int argc, char **argv)
{
  if (argc != 4) {
    cerr << "Usage: "<< argv[0] << " <IP> <port> <buf_addr>" <<endl;
    return 1;
  }
  char *temp[2];

  read_buf_cmd_not_print(argv[1], atoi(argv[2]), temp, true, atoi(argv[3]));

  printf("%.2x %.2x\n",(unsigned char)temp[0],(unsigned char)temp[1]);

  return 0;

}

#endif

