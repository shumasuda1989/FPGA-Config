// fpga_configuration.
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include "mcs_convert.h"
#include "erase_spi.h"
#include "write_file_to_spi.h"
#include "spi_verify.h"
#include "reboot_start.h"

using namespace std;
template <typename T>
vector<T> getlist(ifstream&);

int main()
{
  ifstream fin("config_folder/config_file.txt");
  if(!fin){
    cerr << "Cannot find configfile" << endl;
    return -1;
  }
  vector<string>  mcsfile_list=getlist<string>(fin);
  vector<string>  ip_addr_list=getlist<string>(fin);
  vector<unsigned int>     udpport_list=getlist<unsigned int>(fin);

  // for(int i=0;i<mcsfile_list.size();i++) cout << mcsfile_list[i] << " ";
  // cout << endl;
  // for(int i=0;i<ip_addr_list.size();i++) cout << ip_addr_list[i] << " ";
  // cout << endl;
  // for(int i=0;i<udpport_list.size();i++) cout << udpport_list[i] << " ";
  // cout << endl;
   
  unsigned int udpport = udpport_list[0];
   
  string mcsfile(mcsfile_list[0]); cout << mcsfile << endl;
  string ip_addr(ip_addr_list[0]); cout << ip_addr << endl;
  cout << udpport << endl;
   
  fin.close();

  cout << "config start" << endl;

  time_t start_d = time(0);

  string convert_judge;
  cout << "Do you pass convert? (No/Yes) : " << flush;
 ERROR_1:
  cin >> convert_judge;

  if (convert_judge == "No") 
    mcs_convert(mcsfile.c_str());
  else if (convert_judge == "Yes") 
    cout << "pass convert" << endl;
  else {
    cout << "please input Yes or No : " << flush;
    goto ERROR_1;
  }
  cout << "Erase start" << endl;
  erase_spi(ip_addr.c_str(), udpport);

  cout << "Write Start" <<endl;
  int write_file_size = write_file_to_spi(ip_addr.c_str(), udpport, mcsfile.c_str());
  cout << "Write Finished" <<endl;

  cout << "Verify Start" << endl;
  spi_verify(ip_addr.c_str(), udpport, write_file_size, mcsfile.c_str());

  string reboot_judge;
  cout << "Do you reboot FPGA? (Yes/No) : " << flush;
 ERROR_2:
  cin >> reboot_judge;
  
  if (reboot_judge == "Yes")
    reboot_start(ip_addr.c_str(), udpport);
  else if (reboot_judge == "No")
    cout << "Writing filedata to SPI is finished." << endl;
  else {
    cout << "please input Yes or No : " << flush;
    goto ERROR_2;
  }

  cout << "Writing filedata to SPI finished." << endl;
  cout << "Please wait until FPGA reboot finished." << endl;

  time_t end_d = time(0);
  cout << "start: " << ctime(&start_d) << flush;
  cout << "end:   " << ctime(&end_d) << flush;

}

template <typename T>
vector<T> getlist(ifstream &fin)
{
  vector<T> v;
  string buf;
  T buf2;
  getline(fin,buf);
  stringstream ss(buf);
  while(ss>>buf2)
    v.push_back(buf2);

  return v;
}
