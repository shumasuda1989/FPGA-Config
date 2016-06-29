
# filedat_to_buf.py

import sys
import socket


BUFSIZE  = 512
READSIZE = 128
socket.setdefaulttimeout(2.0)

import addr_convert

def filedata_to_buf(ip_addr, port, filename, read_point, buf_addr_offset) :
   
   file = open(filename,"rb")
   
   socket_addr = (ip_addr, port)
   
   file.seek(read_point)
   read_data = file.read(READSIZE)
   
   read_data_len = len(read_data)
   
   if (read_data_len != 0) :
      rbcp_ver = chr(255) # 1Byte
      rbcp_cmd = chr(int('0x80',16))
      rbcp_ID  = chr(0)
      rbcp_len = chr(int(read_data_len))
      
      #int_buf_addr = hex(buf_addr)
      int_buf_addr = hex(0x800 + buf_addr_offset)
      rbcp_addr_list = addr_convert.addr_convert(int_buf_addr)
      
      udpData_list = []
      
      udpData_list.append(rbcp_ver)
      udpData_list.append(rbcp_cmd)
      udpData_list.append(rbcp_ID)
      udpData_list.append(rbcp_len)
      
      udpData_list = udpData_list + rbcp_addr_list
      
      socket_data = "".join(udpData_list)
      socket_data = socket_data + read_data
      
      udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
      
      udp_socket.sendto(socket_data,socket_addr)
      socket_data, socket_addr = udp_socket.recvfrom(BUFSIZE)
      
      recvdata_list = list(socket_data)
      
      if ((ord(recvdata_list[1]) & 0x0F) != int('0x08',16)) :
         print "Receive Error"
         sys.exit(-1)
      
      """
      for j in range(8,8+ord(recvdata_list[3])) :
         tmp_hex = hex(ord(recvdata_list[j]))

         if (tmp_hex == '0x0') :
            tmp_str = '0x00'
         elif (tmp_hex == '0x1') :
            tmp_str = '0x01'
         elif (tmp_hex == '0x2') :
            tmp_str = '0x02'
         elif (tmp_hex == '0x3') :
            tmp_str = '0x03'
         elif (tmp_hex == '0x4') :
            tmp_str = '0x04'
         elif (tmp_hex == '0x5') :
            tmp_str = '0x05'
         elif (tmp_hex == '0x6') :
            tmp_str = '0x06'
         elif (tmp_hex == '0x7') :
            tmp_str = '0x07'
         elif (tmp_hex == '0x8') :
            tmp_str = '0x08'
         elif (tmp_hex == '0x9') :
            tmp_str = '0x09'
         elif (tmp_hex == '0xa') :
            tmp_str = '0x0a'
         elif (tmp_hex == '0xb') :
            tmp_str = '0x0b'
         elif (tmp_hex == '0xc') :
            tmp_str = '0x0c'
         elif (tmp_hex == '0xd') :
            tmp_str = '0x0d'
         elif (tmp_hex == '0xe') :
            tmp_str = '0x0e'
         elif (tmp_hex == '0xf') :
            tmp_str = '0x0f'
         else :
            tmp_str = str(tmp_hex)
         
         if (j % 8 == 7) :
            print tmp_str
   #         print tmp_str.lstrip('0')
         else :
            print tmp_str,
   #         print tmp_str.lstrip('0'),
      """
      
      
      del udpData_list[0:]
      del recvdata_list[0:]
   
   file.close()
   
   return read_data_len
   


if __name__ == '__main__' :
   
   argvs = sys.argv
   argc  = len(argvs)
   
   int_read_point = int(argvs[2])
   
   for j in range(1) :
      a = filedata_to_buf(ip_addr = '192.168.10.16', port = 4660, filename = argvs[1], read_point = int_read_point)
      print a
   
   



