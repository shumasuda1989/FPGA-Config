#ifndef __filedata_to_buf__
#define __filedata_to_buf__

#include <iostream>
#include <fstream>

int filedata_to_buf(const char *ip_addr, unsigned int port, const char *filename, unsigned int read_point, unsigned int buf_addr_offset);

#endif
