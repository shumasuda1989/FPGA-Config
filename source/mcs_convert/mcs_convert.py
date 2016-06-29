
# mcs_convert.py

import sys

def mcs_convert(filename) :
   
   sour_file = "mcsfile_folder/"     + filename
   conv_file = "convert_folder/spi_" + filename
   

   f1 = open(sour_file,"rb")
   f2 = open(conv_file,"wb")
   
   a = range(1000)
   
   counter        = 0
   cmd_start_flag = 0
   
   while 1 :
      for items in range(1000) :
         a[items] = f1.read(1)
         
         if (a[items] == ":") : 
            if (cmd_start_flag == 1) :
               cmd_start_flag = 0
               Byte_Char   = '0x' + a[1] + a[2]
               Record_Type = '0x' + a[7] + a[8]
               
               byte_length = int(Byte_Char,16)
               
               if (Record_Type == '0x00') :
                  for items_in in range(9,9+byte_length*2,2) :
                     b = '0x' + a[items_in] + a[items_in+1]
                     f2.write(chr(int(b,16)))
               
               f1_point = f1.tell()
               f1_start_point = f1_point - 1
               f1.seek(f1_start_point, 0)
               
               break
            elif (cmd_start_flag == 0) :
               cmd_start_flag = 1
            
         
         if (a[items] == "") : 
            if (cmd_start_flag == 1) :
               cmd_start_flag = 0
               Byte_Char   = '0x' + a[1] + a[2]
               Record_Type = '0x' + a[7] + a[8]
               
               byte_length = int(Byte_Char,16)
               
               if (Record_Type == '0x01') :
                  break
               else :
                  print "MCS File Read Error"
                  f1.close()
                  f2.close()
                  sys.exit(-1)
               
         
      
      counter = counter + 1
      if ((counter % 1000) == 0) :
         print counter
      
      if (Record_Type == '0x01') :
         print "All Finished."
         break
         
   
   f1.close()
   f2.close()


if __name__ == '__main__' :
   
   argvs = sys.argv
   argc  = len(argvs)
   
   if (argc != 2) :
      print "Usage: %s <source mcs file name>" % argvs[0]
   
   mcs_convert(filename = argvs[1])
   

