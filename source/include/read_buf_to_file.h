#ifndef __READ_BUF_TO_FILE__
#define __READ_BUF_TO_FILE__

#include <iostream>
#include <fstream>

void read_buf_to_file(const char *ip_addr, unsigned int port, unsigned int buf_addr = 0x800, unsigned int data_length = 250, const char *filename = "convert_folder/verify_file.txt") ;

#endif
