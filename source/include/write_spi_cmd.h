#ifndef __write_spi_cmd__
#define __write_spi_cmd__

#include <iostream>
#include <fstream>

void write_spi_cmd(const char *ip_addr, unsigned int port, unsigned int spi_reg_addr=0, unsigned int data_length=259);

#endif
