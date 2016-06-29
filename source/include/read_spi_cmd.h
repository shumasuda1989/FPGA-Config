#ifndef __READ_SPI_CMD__
#define __READ_SPI_CMD__

#include <iostream>
#include <fstream>

void read_spi_cmd(const char *ip_addr, unsigned int port, unsigned int spi_reg_addr = 0x0, unsigned int data_length = 260);

#endif
