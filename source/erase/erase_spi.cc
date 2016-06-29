
// erase_spi

#include "wren_cmd.h"
#include "erase_cmd.h"
#include "status_cmd.h"
#include "read_buf_cmd.h"
#include "erase_spi.h"

using namespace std;

void erase_spi(const char *ip_addr, unsigned int port)
{
  
  status_cmd(ip_addr, port);

  wren_cmd(ip_addr, port);
  
  int not_wren_counter = 0;

  while(1)
    {
      
      status_cmd(ip_addr, port);

      char status_list[2];
      read_buf_cmd(ip_addr, port, status_list, true, 8);
      
      if (status_list[0]!=0x2){
	cout << "Not Write Enable" << endl;
	not_wren_counter ++;
      }
      else {
	cout << "Write Enable OK" << endl;
	break;
      }
      
      if (not_wren_counter==3) {
	cerr << "Write Enable Error" << endl;
	exit(-1);
      }
    }

  erase_cmd(ip_addr, port);
   
  while(1)
    {
      sleep(5);

      status_cmd(ip_addr, port);

      char status_list[2];
      read_buf_cmd(ip_addr, port, status_list, true, 8);

      if (status_list[0]==0x3)
	cout << "During erase" << endl;
      else if (status_list[0]==0x0){
	cout << "Erase Finished" << endl;
	break;
      }

    }

}
