
// filedat_to_buf

#include "rbcp_com.h"
#include "filedata_to_buf.h"

using namespace std;
// BUFSIZE  = 512
#define READSIZE 128

int filedata_to_buf(const char *ip_addr, unsigned int port, const char *filename, unsigned int read_point, unsigned int buf_addr_offset) 
{

  ifstream file(filename, ios::in | ios::binary);
  if(!file){
    cerr << "Cannot open " << filename << endl;
    return false;
  }
   
  file.seekg(read_point);
  char *read_data = new char[READSIZE];

  file.read(read_data,READSIZE);

  unsigned int read_data_len = file.gcount();

  if (read_data_len != 0) 
    {
      
      char recvData[UDP_BUF_SIZE];
      char dispMode=0;
      struct rbcp_header sndHeader;

      sndHeader.type=RBCP_VER;
      sndHeader.command= RBCP_CMD_WR;
      sndHeader.id=0;
      sndHeader.length=read_data_len;
      sndHeader.address=htonl(0x800+ buf_addr_offset); 

      int rcvdBytes;
      rcvdBytes= rbcp_com(ip_addr, port, &sndHeader, read_data, recvData, dispMode);

      if(rcvdBytes<0) exit(EXIT_FAILURE);

      if ( (recvData[1] & 0x0F) != 0x08 ) {
	cerr << "Receive Error (filedata_to_buf)" << endl;
	exit(-1);
      }
    }
   
  file.close();
  delete[] read_data;
  
  return read_data_len;

}
   

#ifdef TEST_FILEDATA_TO_BUF

int main(int argc,char **argv)
{
  if (argc != 2) {
    cerr << "Usage: "<< argv[0] << " <IP> <port> <conv file name> <read point>" <<endl;
    return 1;
  }
  int a = filedata_to_buf(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
  cout << a << endl;

  return 0;

}

#endif
