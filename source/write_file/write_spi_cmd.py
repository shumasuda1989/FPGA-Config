
# write_spi_cmd.py

import sys
import socket
import addr_convert

def write_spi_cmd(ip_addr = '192.168.10.16', port = 4660, spi_reg_addr = 0, data_length = 259 ) :

   BUFSIZE = 512
   socket_addr = (ip_addr, port)
   socket.setdefaulttimeout(2.0)
   
   rbcp_ver = chr(255) # 1Byte
   rbcp_cmd = chr(int('0x80',16))
   rbcp_ID  = chr(0)
   rbcp_len = chr(7)
   
   int_hex_data_length = hex(data_length)
   data_length_list = addr_convert.addr_convert(int_hex_data_length)
   
   int_hex_addr = hex(spi_reg_addr)
   spi_addr_list = addr_convert.addr_convert(int_hex_addr)
   
   udpData_list = []
   
   udpData_list.append(rbcp_ver)
   udpData_list.append(rbcp_cmd)
   udpData_list.append(rbcp_ID)
   udpData_list.append(rbcp_len)
   
   udpData_list.append(chr(int('0x00',16)))
   udpData_list.append(chr(int('0x00',16)))
   udpData_list.append(chr(int('0x00',16)))
   udpData_list.append(chr(int('0x05',16)))
   
   udpData_list.append(chr(int('0x83',16)))
   
   udpData_list = udpData_list + data_length_list[2:4]
   #udpData_list.append(chr(int('0x00',16)))
   #udpData_list.append(chr(int(data_length)))
   
   udpData_list.append(chr(int('0x02',16)))
   
   udpData_list = udpData_list + spi_addr_list[1:4]
   """
   udpData_list.append(chr(int('0x00',16)))
   udpData_list.append(chr(int('0x00',16)))
   udpData_list.append(chr(int('0x40',16)))
   """
   
   socket_data = "".join(udpData_list)
   
   udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   
   udp_socket.sendto(socket_data,socket_addr)
   socket_data, socket_addr = udp_socket.recvfrom(BUFSIZE)
   
   recvdata_list = list(socket_data)
   
   if ((ord(recvdata_list[1]) & 0x0F) != int('0x08',16)) :
      print "Receive Error"
      sys.exit(-1)
   
   del udpData_list[0:]
   del recvdata_list[0:]


if __name__ == '__main__' :
   write_spi_cmd()

