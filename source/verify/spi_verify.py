
# spi_verify.py

import sys
import time

import read_spi_cmd
import read_buf_to_file

def spi_verify(ip_addr, port, file_size, conv_file_name) :
      
   file = open("convert_folder/verify_file.txt","wb")
   file.close()
   
   read_addr = 0
   
   while (read_addr < file_size) :
      
      read_spi_cmd.read_spi_cmd(ip_addr = ip_addr, port = port, spi_reg_addr = read_addr)
      
      time.sleep(0.005)
      
      read_buf_to_file.read_buf_to_file(ip_addr = ip_addr, port = port)
      
      read_addr = read_addr + 250
      
   
   conv_source_file = "convert_folder/spi_" + conv_file_name
   
   file1 = open(conv_source_file,"rb")
   file2 = open("convert_folder/verify_file.txt","rb")
   
   byte_counter = 0
   
   while 1 :
      comp1 = file1.read(1)
      comp2 = file2.read(1)
      
      if (comp1 == "") :
         print byte_counter
         break
      
      ord_comp1 = ord(comp1)
      ord_comp2 = ord(comp2)
      
      if ((ord_comp1 - ord_comp2) != 0) :
         print "Verify False"
         print byte_counter
         file1.close()
         file2.close()
         sys.exit(-1)
      
      byte_counter = byte_counter + 1
      
      if ((byte_counter % 10000) == 0) :
         print byte_counter
   
   file1.close()
   file2.close()
   
   print "Verify Success"
   


if __name__ == '__main__' :
   
   argvs = sys.argv
   argc  = len(argvs)
   
   if (argc != 2) :
      print "Usage : %s <source file>" % argvs[0]
      sys.exit(-1)
      
   
   spi_verify(ip_addr = '192.168.10.16', port = 4660, file_size = 341618 ,conv_file_name = argvs[1])


