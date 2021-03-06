
# read_buf_to_file.py

import sys
import socket
import addr_convert


def read_buf_to_file(ip_addr = '192.168.10.16', port = 4660, buf_addr = 0x800, data_length = 250, filename = "convert_folder/verify_file.txt") :
   
   BUFSIZE = 512
   socket_addr = (ip_addr, port)
   socket.setdefaulttimeout(2.0)
   
   rbcp_ver = chr(255) # 1Byte
   rbcp_cmd = chr(int('0xC0',16))
   rbcp_ID  = chr(0)
   rbcp_len = chr(int(data_length))
   
   int_buf_addr = hex(buf_addr)
   rbcp_addr_list = addr_convert.addr_convert(int_buf_addr)
   
   udpData_list = []
   
   udpData_list.append(rbcp_ver)
   udpData_list.append(rbcp_cmd)
   udpData_list.append(rbcp_ID)
   udpData_list.append(rbcp_len)
   
   udpData_list = udpData_list + rbcp_addr_list
   
   socket_data = "".join(udpData_list)
   
   udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   
   udp_socket.sendto(socket_data,socket_addr)
   socket_data, socket_addr = udp_socket.recvfrom(BUFSIZE)
   
   recvdata_list = list(socket_data)
   
   if ((ord(recvdata_list[1]) & 0x0F) != int('0x08',16)) :
      print "Receive Error"
      sys.exit(-1)
   
   file = open(filename,"ab")
   
   for j in range(8,8+ord(recvdata_list[3])) :
      file.write(recvdata_list[j])
   
   file.close()
   

if __name__ == '__main__' :
   read_buf_cmd(data_length = 250)

