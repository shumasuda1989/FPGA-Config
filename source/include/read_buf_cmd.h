#ifndef __read_buf_cmd_not_print__
#define __read_buf_cmd_not_print__

#include <iostream>
#include <fstream>

void read_buf_cmd(const char *ip_addr, unsigned int port, char *status_list, bool showprint=false, unsigned int buf_addr = 0x800, unsigned int data_length = 128);

#endif
