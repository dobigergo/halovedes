#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#define buff_size 1024
#define port_no 3000

int main(int argc, char *argv[]){

   int fd;
   struct sockaddr_in server;	   
   struct sockaddr_in client;	     
   int server_size=sizeof server;                 
   int client_size=sizeof client;               
   int bytes;    	               
   int rcvsize;                    
   int trnmsize;                   
   int err;  
   int ertek;                  
   				
   char on=1;                         
   char buffer[buff_size+1];        
   char server_addr[16];

   sprintf(server_addr,"%s", argv[1]);
   int ip;
   ip = inet_addr(server_addr);	
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = ip;
   server.sin_port = htons(port_no);



   fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
      printf("%s: Socket creation error.\n",argv[0]);
      exit(1);
   }
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   
   err = connect(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      printf("%s: Cannot connect to the server.\n", argv[0]);
      exit(2);
   }


 strcpy(buffer,"Új kör kezdődik"); 
 printf("%s\n",buffer);
   


//POKER MECCS KEZDETE

while(strcmp(buffer,"Új kör kezdődik")==0){

   
   int cnt=0;
   while(cnt!=5){
   printf("üzenet fogadása:\n");
   bytes=buff_size+1;
   rcvsize = recv( fd, buffer, bytes, 0 );
   if (rcvsize < 0) {
      printf("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(4);
   }
   printf("%s\n",buffer);
   cnt++;
   }



   	printf("Csere számok elküldése\n");
	
	ertek=1;
	cnt=0;
	while(ertek!=0 && cnt !=3){
	scanf("%d",&ertek);
	bytes=sizeof(ertek)+1;
	send(fd,&ertek,bytes,0);
	cnt++;
	}

   //Csere számok fogadása

   cnt=0;
   bytes=buff_size+1;
   while(cnt!=5){
   printf("üzenet fogadása:\n");
   rcvsize = recv( fd, buffer, bytes, 0 );
   if (rcvsize < 0) {
      printf("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(4);
   }
   printf("%s\n",buffer);
   cnt++;
   }
	
	printf("Az eredmény:\n");

	rcvsize = recv( fd, buffer, bytes, 0 );
   if (rcvsize < 0) {
      printf("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(4);
   }
   printf("%s\n",buffer);

   printf("Feladáshoz írd be hogy feladom új kör indításához azt hogy kész\n");
   scanf("%s",buffer);
   bytes=strlen(buffer)+1;
   send(fd,buffer,bytes,0);

   printf("Urolsó rcv a körben\n");
   
   bytes=buff_size+1;
   rcvsize=recv(fd,buffer,bytes,0);
   if (rcvsize < 0) {
      printf("%s: Cannot receive data from the socket.\n", argv[0]);
      exit(4);
   }
   printf("%s\n",buffer);


}
 
  close(fd);
   exit(0);
}
