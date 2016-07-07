
// spi_verify

#include "read_spi_cmd.h"
#include "read_buf_to_file.h"
#include "spi_verify.h"

using namespace std;

void spi_verify(const char *ip_addr, unsigned int port, unsigned int file_size, const char *conv_file_name)
{

  string verfile("convert_folder/verify_file_");
  verfile+=ip_addr; verfile+=".txt";
  ofstream file(verfile.c_str(),ios::out|ios::binary|ios::trunc);
  if(!file){
    cerr << "Cannot open "<< verfile << endl;
    return;
  }
  file.close();

  unsigned int read_addr = 0;
   
  while (read_addr < file_size) 
    {

      read_spi_cmd(ip_addr, port, read_addr);

      usleep(5000);

      read_buf_to_file(ip_addr, port,0x800,250,verfile.c_str());

      read_addr += 250;

    }

  string conv_source_file("convert_folder/spi_");
  conv_source_file+= conv_file_name;

  ifstream file1(conv_source_file.c_str(), ios::in | ios::binary);
  if(!file1){
    cerr << "Cannot open " << conv_source_file << endl;
    return;
  }
  ifstream file2(verfile.c_str(), ios::in | ios::binary);

  int byte_counter = 0;

  while(1)
    {
      char comp1,comp2;

      file1.read(&comp1,1);
      file2.read(&comp2,1);

      if (file1.gcount()==0){
	cout << byte_counter <<endl;
	break;
      }
      
      if ((comp1 - comp2) != 0){
	cerr << "Verify False" << endl;
	cerr << byte_counter << endl;
	file1.close();
	file2.close();
	exit(-1);
      }

      byte_counter++;

      if ((byte_counter % 10000) == 0)
	cout <<  byte_counter <<endl;
    }
  file1.close();
  file2.close();
   
  cout <<"Verify Success" <<endl;

}


#ifdef TEST_SPI_VERIFY
int main(int argc, char **argv)
{
  if (argc !=5){
    cout << "Usage : " << argv[0] << "<IP> <port> <size> <source file>" <<endl;
    exit(-1);
  }  

  spi_verify(argv[1], atoi(argv[2]), atoi(argv[3]) ,argv[4]);

  return 0;
}

#endif


