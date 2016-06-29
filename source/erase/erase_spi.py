
#  erase_spi.py

import wren_cmd
import erase_cmd
import status_cmd
import read_buf_cmd

import sys
import time

def erase_spi(ip_addr = '192.168.10.16', port = 4660) :
   
   status_cmd.status_cmd(ip_addr = ip_addr, port = port)
   
   wren_cmd.wren_cmd(ip_addr = ip_addr, port = port)
   
   not_wren_counter = 0
   
   while 1 :
      
      status_cmd.status_cmd(ip_addr = ip_addr, port = port)
      
      status_list = read_buf_cmd.read_buf_cmd(ip_addr = ip_addr, port = port, data_length = 8)
      
      if (status_list[0]!='0x2') :
         print "Not Write Enable"
         not_wren_counter = not_wren_counter + 1
      elif (status_list[0]=='0x2') :
         print "Write Enable OK"
         break
      
      if (not_wren_counter==3) :
         print "Write Enable Error"
         sys.exit(-1)
      
   
   erase_cmd.erase_cmd(ip_addr = ip_addr, port = port)
   
   while 1 :
      
      time.sleep(5.0)
      
      status_cmd.status_cmd(ip_addr = ip_addr, port = port)
      
      status_list = read_buf_cmd.read_buf_cmd(ip_addr = ip_addr, port = port, data_length = 8)
      
      if (status_list[0]=='0x3') :
         print "During erase"
      elif (status_list[0]=='0x0') :
         print "Erase Finished"
         break
      
   


if __name__ == '__main__' :
   erase_spi()

