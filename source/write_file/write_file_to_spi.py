
# write_file_to_spi.py

import sys
import time

import wren_cmd
import filedata_to_buf
import write_spi_cmd
import status_cmd
import read_buf_cmd_not_print

def write_file_to_spi(ip_addr, port, filename) :
   
   finish_flag = 0
   counter     = 0
   total_addr  = 0
   
   conv_source_file = "convert_folder/spi_" + filename
   
   while (finish_flag == 0) :
      wren_cmd.wren_cmd(ip_addr = ip_addr, port = port)
      
      read_data_length1 = 0
      read_data_length2 = 0
      
      read_data_length1 = filedata_to_buf.filedata_to_buf(ip_addr = ip_addr, port = port, filename = conv_source_file, read_point = total_addr, buf_addr_offset = 0)
      total_addr = total_addr + read_data_length1
      
      if (read_data_length1 == 128) :
         read_data_length2 = filedata_to_buf.filedata_to_buf(ip_addr = ip_addr, port = port, filename = conv_source_file, read_point = total_addr, buf_addr_offset = 128)
         total_addr = total_addr + read_data_length2
      
      not_wren_counter = 0
      
      while 1 :
         
         write_spi_cmd.write_spi_cmd(ip_addr = ip_addr, port = port, spi_reg_addr = (total_addr - (read_data_length1 + read_data_length2)), data_length = 259)
         time.sleep(0.003)
         
         status_cmd.status_cmd(ip_addr = ip_addr, port = port)
         
         status_list = read_buf_cmd_not_print.read_buf_cmd_not_print(ip_addr = ip_addr, port = port, data_length = 8)
         
         if (status_list[0]!='0x0') :
            not_wren_counter = not_wren_counter + 1
         elif (status_list[0]=='0x0') :
            break
         
         if (not_wren_counter==3) :
            print "too much time"
            sys.exit(-1)
         
      
      counter  = counter + 1
      
      if ((counter % 100) == 0) :
         print total_addr
      
      if ((total_addr % 256) != 0) :
         finish_flag = 1
         print total_addr
      
   
   return total_addr
   


if __name__ == '__main__' :
   
   argvs = sys.argv
   argc  = len(argvs)
   
   if (argc != 2) :
      print "Usage : %s <source file>" % argvs[0]
      sys.exit(-1)
      
   
   write_file_to_spi(ip_addr = '192.168.10.16', port = 4660, filename = argvs[1])

