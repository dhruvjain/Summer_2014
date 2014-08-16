#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void error(char *);

int main(int argc,char *argv[])
{

   int sock,length,n;
   struct sockaddr_in server,from;
   struct hostent *hp;
   char buffer[256];
   if(argc !=3){
    printf("Usage server port \n");
    exit(1);
  
  }

   sock=socket(AF_INET,SOCK_DGRAM,0);
 
   if(sock<0){
             error("socket");
   }
   server.sin_family=AF_INET;

   hp=gethostbyname(argv[1]);

  if(hp==0){

   error("unknown host");
  }

  bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);

  server.sin_port=htons(atoi(argv[2]));
  length=sizeof(struct sockaddr_in);


  printf("Please enter the password \n");

  bzero(buffer,256);
  fgets(buffer,255,stdin);

  n=sendto(sock,buffer,strlen(buffer),0,&server,length);

  if(n<0){
  error("Sendto");
 }
  
 n=recvfrom(sock,buffer,256,0,&from,&length);

 if(n<0){
  error("recv from");
}
 printf("Got a message : no of uavs\n");
 write(1,buffer,n);
 printf("Please enter the uav id \n");

 bzero(buffer,256);
 fgets(buffer,255,stdin);
 n=sendto(sock,buffer,strlen(buffer),0,&server,length);
 if(n<0){
  error("Sendto");
 }
  
 

}
