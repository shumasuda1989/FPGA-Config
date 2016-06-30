#define RBCP_DISP_MODE_NO 0
#define RBCP_DISP_MODE_INTERACTIVE 1
#define RBCP_DISP_MODE_DEBUG 2
#include "rbcp_com.h"

int rbcp_com(const char* ipAddr, unsigned int port, struct rbcp_header* sendHeader, char* sendData, char* recvData, char dispMode){

   struct sockaddr_in sitcpAddr;
   int sock;

  struct timeval timeout;
  fd_set setSelect;
  
  int sndDataLen;
  int cmdPckLen;

  char sndBuf[1024];
  int i, j = 0;
  int rcvdBytes;
  char rcvdBuf[1024];
  int numReTrans =0;

  /* Create a Socket */
  if(dispMode==RBCP_DISP_MODE_DEBUG) puts("\nCreate socket...\n");

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  sitcpAddr.sin_family      = AF_INET;
  sitcpAddr.sin_port        = htons(port);
  sitcpAddr.sin_addr.s_addr = inet_addr(ipAddr);

  sndDataLen = (int)sendHeader->length;

  if(dispMode==RBCP_DISP_MODE_DEBUG) printf(" Length = %i\n",sndDataLen);

  /* Copy header data */
  memcpy(sndBuf,sendHeader, sizeof(struct rbcp_header));

  if(sendHeader->command==RBCP_CMD_WR){
    memcpy(sndBuf+sizeof(struct rbcp_header),sendData,sndDataLen);
    cmdPckLen=sndDataLen + sizeof(struct rbcp_header);
  }else{
    cmdPckLen=sizeof(struct rbcp_header);
  }


  if(dispMode==RBCP_DISP_MODE_DEBUG){
    for(i=0; i< cmdPckLen;i++){
      if(j==0) {
	printf("\t[%.3x]:%.2x ",i,(unsigned char)sndBuf[i]);
	j++;
      }else if(j==3){
	printf("%.2x\n",(unsigned char)sndBuf[i]);
	j=0;
      }else{
	printf("%.2x ",(unsigned char)sndBuf[i]);
	j++;
      }
    }
    if(j!=3) printf("\n");
  }

  /* send a packet*/

  sendto(sock, sndBuf, cmdPckLen, 0, (struct sockaddr *)&sitcpAddr, sizeof(sitcpAddr));
  if(dispMode==RBCP_DISP_MODE_DEBUG) puts("The packet have been sent!\n");

  /* Receive packets*/
  
  if(dispMode==RBCP_DISP_MODE_DEBUG) puts("\nWait to receive the ACK packet...");

  if(dispMode==3) return 0; //Shu added

  while(numReTrans<3){

    FD_ZERO(&setSelect);
    FD_SET(sock, &setSelect);

    timeout.tv_sec  = 2;//changed from 1 by Shu
    timeout.tv_usec = 0;
 
    if(select(sock+1, &setSelect, NULL, NULL,&timeout)==0){
      /* time out */
      puts("\n***** Timeout ! *****");
      sendHeader->id++;
      memcpy(sndBuf,sendHeader, sizeof(struct rbcp_header));
      sendto(sock, sndBuf, cmdPckLen, 0, (struct sockaddr *)&sitcpAddr, sizeof(sitcpAddr));
      numReTrans++;
      FD_ZERO(&setSelect);
      FD_SET(sock, &setSelect);
    } else {
      /* receive packet */
      if(FD_ISSET(sock,&setSelect)){
	rcvdBytes=recvfrom(sock, rcvdBuf, 2048, 0, NULL, NULL);

	if(rcvdBytes<sizeof(struct rbcp_header)){
	  puts("ERROR: ACK packet is too short");
	  close(sock);
	  return -1;
	}

	if((0x0f & rcvdBuf[1])!=0x8){
	  puts("ERROR: Detected bus error");
	  close(sock);
	  return -1;
	}

	rcvdBuf[rcvdBytes]=0;

	if(RBCP_CMD_RD){
	  //memcpy(recvData,rcvdBuf+sizeof(struct rbcp_header),rcvdBytes-sizeof(struct rbcp_header));
	  memcpy(recvData,rcvdBuf,rcvdBytes); //Shu changed
	}

	if(dispMode==RBCP_DISP_MODE_DEBUG){
	  puts("\n***** A pacekt is received ! *****.");
	  puts("Received data:");

	  j=0;

	  for(i=0; i<rcvdBytes; i++){
	    if(j==0) {
	      printf("\t[%.3x]:%.2x ",i,(unsigned char)rcvdBuf[i]);
	      j++;
	    }else if(j==3){
	      printf("%.2x\n",(unsigned char)rcvdBuf[i]);
	      j=0;
	    }else{
	      printf("%.2x ",(unsigned char)rcvdBuf[i]);
	      j++;
	    }
	    if(i==7) printf("\n Data:\n");
	  }

	  if(j!=3) puts(" ");
	}else if(dispMode==RBCP_DISP_MODE_INTERACTIVE){
	  if(sendHeader->command==RBCP_CMD_RD){
	    j=0;
	    puts(" ");

	    for(i=8; i<rcvdBytes; i++){
	      if(j==0) {
		printf(" [0x%.8x] %.2x ",ntohl(sendHeader->address)+i-8,(unsigned char)rcvdBuf[i]);
		j++;
	      }else if(j==7){
		printf("%.2x\n",(unsigned char)rcvdBuf[i]);
		j=0;
	      }else if(j==4){
		printf("- %.2x ",(unsigned char)rcvdBuf[i]);
		j++;
	      }else{
		printf("%.2x ",(unsigned char)rcvdBuf[i]);
		j++;
	      }
	    }
	    
	    if(j!=15) puts(" ");
	  }/* else { */
	  /*   printf(" 0x%x: OK\n",ntohl(sendHeader->address)); */
	  /* } */
	}
	numReTrans = 4;
	close(sock);
	return(rcvdBytes);
      }
    }
  }
  close(sock);

  return -3;
}
