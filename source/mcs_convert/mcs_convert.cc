
// mcs_convert
#include "mcs_convert.h"

using namespace std;

bool mcs_convert(const char *filename)
{

  string mcsfolder="mcsfile_folder/";
  ifstream f1( (mcsfolder+filename).c_str(), ios::in | ios::binary);
  if(!f1){
    cerr << "Cannot open " << (mcsfolder+filename).c_str() << endl;
    return false;
  }
  string convfolder="convert_folder/spi_";
  ofstream f2( (convfolder+filename).c_str(),ios::binary);
  if(!f2){
    cerr << "Cannot open " << (convfolder+filename).c_str() << endl;
    return false;
  }

  const int MAXBYTE=1000;
  char *a = new char[MAXBYTE];

  int counter        = 0;
  bool cmd_start_flag = false;

  while(1)
    {
      char Byte_Char[5]="";
      char Record_Type[5]="";
      int byte_length;

      for(int items=0;items<MAXBYTE;items++)
	{
	  f1.read(&a[items],1);

	  if (a[items] == ':')  {
            if (cmd_start_flag)
	      {
		cmd_start_flag = false;
		strcpy(Byte_Char,"0x");
		strncat(Byte_Char,&a[1],2);
		strcpy(Record_Type,"0x");
		strncat(Record_Type,&a[7],2);

		byte_length = strtol(Byte_Char,0,16);

		if (strcmp(Record_Type, "0x00")==0)
                  for(int items_in=9;items_in<9+byte_length*2;items_in+=2)
		    {
		      char b[5];
		      strcpy(b,"0x");
		      strncat(b,&a[items_in],2);
		      char buf=(char)strtol(b,0,16);
		      f2.write(&buf,1);
		    }

		unsigned int f1_point = f1.tellg();
		unsigned int f1_start_point = f1_point - 1;
		f1.seekg(f1_start_point, ios_base::beg);

		break;
	      }
            else //if (!cmd_start_flag)
	      cmd_start_flag = true;

	  }

	  //if (a[items] == '') 
	  if (f1.eof()) 
            if (cmd_start_flag)
	      {
		cmd_start_flag = false;
		strcpy(Byte_Char,"0x");
		strncat(Byte_Char,&a[1],2);
		strcpy(Record_Type,"0x");
		strncat(Record_Type,&a[7],2);

		byte_length = strtol(Byte_Char,0,16);

		if (strcmp(Record_Type,"0x01")==0)
                  break;
		else{
                  cerr << "MCS File Read Error" << endl;
                  f1.close();
                  f2.close();
                  return false;
		}
	      }               
	}

      counter++;
      if ((counter % 1000) == 0)
	cout << counter << endl;

      if (strcmp(Record_Type, "0x01")==0){
	cout << "All Finished." << endl;
	break;
      }

    }   

  delete[] a;
  f1.close();
  f2.close();

  return true;

}


#ifdef TEST_MCS_CONVERT

int main(int argc,char **argv)
{
  if (argc != 2) {
    cerr << "Usage: "<< argv[0] << " <source mcs file name>" <<endl;
    return 1;
  }
  char *filename=argv[1];

  if(mcs_convert(filename))
    return 0;
  else
    return 1;

}

#endif
