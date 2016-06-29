
# addr_convert.py

import sys
import copy

def addr_convert(Hex_String):
   
   String_Len  = len(Hex_String)
   String_List = list(Hex_String)
   
   if ((String_Len > 10) or (String_Len < 3)) :
      print "Length Error"
      sys.exit(-1)
   
   tmp_Addr = range(8)
   
   if (String_List[0:2]==['0','x']) :
      String_Len_copy = copy.copy(String_Len)
      
      i = 7
      while (i > -1) :
         if (String_List[String_Len_copy-1]=='x') :
            tmp_Addr[i] = '0'
            i = i - 1
         else :
            tmp_Addr[i]     = String_List[String_Len_copy-1]
            String_Len_copy = String_Len_copy - 1
            i = i - 1
      
      Hex_Addr1 = '0x' + "".join(tmp_Addr[0:2])
      Hex_Addr2 = '0x' + "".join(tmp_Addr[2:4])
      Hex_Addr3 = '0x' + "".join(tmp_Addr[4:6])
      Hex_Addr4 = '0x' + "".join(tmp_Addr[6:8])
      
      Addr1 = chr(int(Hex_Addr1,16))
      Addr2 = chr(int(Hex_Addr2,16))
      Addr3 = chr(int(Hex_Addr3,16))
      Addr4 = chr(int(Hex_Addr4,16))
      
   else :
      print "Please input Address by hex"
      sys.exit(-1)
   
   Addr_List = []
   
   Addr_List.append(Addr1)
   Addr_List.append(Addr2)
   Addr_List.append(Addr3)
   Addr_List.append(Addr4)
   
   return Addr_List


if __name__ == '__main__' :
   
   argvs = sys.argv
   argc  = len(argvs)
   
   if (argc!=2) :
      print "Usage: %s <Hex Address>" % argvs[0]
   
   test_List = [1,2]
   
   test_List = test_List + addr_convert(argvs[1])
   
   print test_List

