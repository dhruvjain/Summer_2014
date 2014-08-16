#include <stdio.h>

#include <errno.h>

#include <string.h>

#include <stdlib.h>

#include <pthread.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <fcntl.h>

#include <termios.h>

#include <sys/ioctl.h>

void error(char* msg){

perror(msg);
exit(0);
}

FILE *fp;

void * process(void * ptr)
{
        //char c[100];
        //c=(char *)ptr;
        fp = popen(ptr,"r");

        pclose(fp);
        
}

int main(){

  int sock,length,fromlen,n;
  pthread_t thread;
  struct sockaddr_in server;
  struct sockaddr_in from;
  char buf[1024];

  
  sock=socket(AF_INET,SOCK_DGRAM,0);

  if(sock<0){
   perror("Opening port");
  }

  length=sizeof(server);

  bzero(&server,length);
  server.sin_family = AF_INET;

  server.sin_addr.s_addr = INADDR_ANY;

  server.sin_port = htons(8000);

  if (-1 == bind(sock,(struct sockaddr *)&server,length))

{

      perror("error bind failed");

      close(sock);

      exit(1);
} 

fromlen= sizeof(struct sockaddr_in);
int i=0;
char printbuf[INET_ADDRSTRLEN];
int vlcport;
while(1){

          n=recvfrom(sock,buf,1024,0,(struct sockadd*)&from,&fromlen);
        
          if(n<0){
           error("recvfrom");
          }


          write(1,"Received a datagram:",21);
          write(1,buf,n);

        
          inet_ntop(from.sin_family, &from.sin_addr.s_addr, printbuf, sizeof(printbuf));

 
          vlcport=8080+i;

          char port[15];
          sprintf(port, "%d", vlcport);
	  char command[200]="cvlc http://";
	  strcat(command,printbuf);
          strcat(command,":8080 --sout '#transcode{vcodec=mp4v,acodec=mpga,vb=800,ab=128}:standard{access=http,mux=asf,dst=:");

          strcat(command,port);
          strcat(command," --no-sout-audio");
 
          fp = popen(command,"r");

           pclose(fp);
          
          //pthread_create(&thread, 0, process, (void *) command);
	  //pthread_detach(thread);
	 

          i++;
    }

}


