
# fpga_configuration.py

import sys
import datetime

sys.path.append('source/mcs_convert')
sys.path.append('source/erase')
sys.path.append('source/write_file')
sys.path.append('source/verify')
sys.path.append('source/reboot')

import mcs_convert
import erase_spi
import write_file_to_spi
import spi_verify
import reboot_start

def fpga_configuration() :
   
   config_file = open("config_folder/config_file.txt","r")
   
   mcsfile_list = list(config_file.readline())
   ip_addr_list = list(config_file.readline())
   udpport_list = list(config_file.readline())
   """
   print mcsfile_list
   print ip_addr_list
   print udpport_list
   """
   mcsfile_list.remove('\n')
   mcsfile_list.remove('\r')
   ip_addr_list.remove('\n')
   ip_addr_list.remove('\r')
   udpport_list.remove('\n')
   udpport_list.remove('\r')
   
   mcsfile = "".join(mcsfile_list)
   ip_addr = "".join(ip_addr_list)
   udpport = "".join(udpport_list)
   
   int_udpport = int(udpport)
   
   print mcsfile
   print ip_addr
   print int_udpport
   
   config_file.close()
   
   print "config start"

   start_d   = datetime.datetime.today()
   
   
   convert_judge = raw_input('Do you pass convert? (No/Yes) : ')
   
   if (convert_judge == 'No') :
      mcs_convert.mcs_convert(filename = mcsfile)
   elif (convert_judge == 'Yes') :
      print "pass convert"
   else :
      print "please input Yes or No"
   
   
   erase_spi.erase_spi(ip_addr = ip_addr, port = int_udpport)
   
   write_file_size = write_file_to_spi.write_file_to_spi(ip_addr = ip_addr, port = int_udpport, filename = mcsfile)
   
   spi_verify.spi_verify(ip_addr = ip_addr, port = int_udpport, file_size = write_file_size ,conv_file_name = mcsfile)
   
   
   reboot_judge = raw_input('Do you reboot FPGA? (Yes/No) : ')
   
   if (reboot_judge == 'Yes') :
      reboot_start.reboot_start(ip_addr = ip_addr, port = int_udpport)
   elif (reboot_judge == 'No') :
      print "Writing filedata to SPI is finished."
   else :
      print "please input Yes or No"
   
   print "Writing filedata to SPI finished."
   print "Please wait until FPGA reboot finished."

   end_d = datetime.datetime.today()
   print start_d
   print end_d



if __name__ == '__main__' :
   fpga_configuration()


