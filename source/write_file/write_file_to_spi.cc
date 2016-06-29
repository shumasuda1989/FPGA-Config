
// write_file_to_spi

#include "wren_cmd.h"
#include "filedata_to_buf.h"
#include "write_spi_cmd.h"
#include "status_cmd.h"
#include "read_buf_cmd.h"
#include "write_file_to_spi.h"

using namespace std;

int write_file_to_spi(const char *ip_addr, unsigned int port, const char *filename) 
{   
  bool finish_flag = false;
  int  counter     = 0;
  int  total_addr  = 0;
   
  string conv_source_file("convert_folder/spi_");
  conv_source_file += filename;
   
  while (!finish_flag) 
    {
      wren_cmd(ip_addr, port);
      
      int read_data_length1 = 0;
      int read_data_length2 = 0;
      
      read_data_length1 = filedata_to_buf(ip_addr, port, conv_source_file.c_str(), total_addr, 0);
      total_addr += read_data_length1;
      
      if (read_data_length1 == 128) 
	{
	  read_data_length2 = filedata_to_buf(ip_addr, port, conv_source_file.c_str(), total_addr, 128);
	  total_addr += read_data_length2;
	}
      int not_wren_counter = 0;
      
      while (1) 
	{
         
	  write_spi_cmd(ip_addr, port, (total_addr - (read_data_length1 + read_data_length2)), 259);
	  usleep(3000);
         
	  status_cmd(ip_addr, port);
         
	  char status_list[2];
	  read_buf_cmd(ip_addr, port, status_list, false, 8);

	  if (status_list[0]!=0x0) 
            not_wren_counter ++;
	  else
            break;
         
	  if (not_wren_counter==3){
            cerr << "too much time" << endl;
            exit(-1);
	  }
	}
      counter ++;
      
      if ((counter % 100) == 0)
	cout << total_addr <<endl;
      
      if ((total_addr % 256) != 0){
	finish_flag = true;
	cout << total_addr <<endl;
      }
    }
      
   
  return total_addr;
   
}


#ifdef TEST_WRITE_FILE_TO_SPI

int main(int argc, char **argv)
{
   
  if (argc != 4) {
    printf( "Usage : %s <IP> <port> <source file>" , argvs[0]);
    return -1;
  }

  return write_file_to_spi(argv[1], atoi(argv[2]), argvs[3]);

}

#endif
