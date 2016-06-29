
# reboot_start.py

import sys
import socket

def reboot_start(ip_addr, port) :

   BufSize   = 256
   sock_Addr = (ip_addr, port)
   socket.setdefaulttimeout(2.0)

   rbcp_ver = chr(255) # 1 Btye
   rbcp_cmd = chr(int('0x80',16)) #Write is 0x80, Read is 0xC0
   rbcp_ID  = chr(0)
   rbcp_len = chr(1)

   rbcp_Addr1 = chr(0)
   rbcp_Addr2 = chr(0)
   rbcp_Addr3 = chr(0)
   rbcp_Addr4 = chr(int('0x1F',16))

   data1 = chr(int('0xFF',16))

   udpData_list = []

   udpData_list.append(rbcp_ver)
   udpData_list.append(rbcp_cmd)
   udpData_list.append(rbcp_ID)
   udpData_list.append(rbcp_len)

   udpData_list.append(rbcp_Addr1)
   udpData_list.append(rbcp_Addr2)
   udpData_list.append(rbcp_Addr3)
   udpData_list.append(rbcp_Addr4)

   udpData_list.append(data1)

   socket_data = "".join(udpData_list)

   udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

   udp_socket.sendto(socket_data,sock_Addr)
   """
   socket_data, sock_Addr = udp_socket.recvfrom(BufSize)

   recv_socket_data_list = list(socket_data)

   if ( (ord(recv_socket_data_list[1]) & 0x0F) != int('0x08',16) ) :
      print "Receive Error"
      sys.exit(-1)]
   """

if __name__ == '__main__' :
   reboot_start(ip_addr = '192.168.10.15', port = 4660)

