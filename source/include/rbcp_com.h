#ifndef __RBCP__
#define __RBCP__

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LINE_LENGTH 1024
#define MAX_PARAM_LENGTH 20
#define RBCP_VER 0xFF
#define RBCP_CMD_WR 0x80
#define RBCP_CMD_RD 0xC0
#define DEFAULT_IP 192.168.0.16
#define UDP_BUF_SIZE 2048

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>


struct rbcp_header{
  unsigned char type;
  unsigned char command;
  unsigned char id;
  unsigned char length;
  unsigned int address;
};

int rbcp_com(const char* ipAddr, unsigned int port, struct rbcp_header* sendHeader, char* sendData, char* recvData, char dispMode);


#ifdef __cplusplus
}
#endif

#endif
