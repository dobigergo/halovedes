#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include "poker.h"

#define buff_size 1024
#define port_no 3000

int main(int argc, char** argv){
struct card deck[32];
struct card hand[5];
struct card hand2[5];
int c=0;
int fd;
int fdc;
int fdc2;
struct sockaddr_in server;
struct sockaddr_in client;
struct sockaddr_in client2;
int server_size=sizeof server;
int client_size=sizeof client;
int client_size2=sizeof client2;
int bytes;
int rcvsize;
int trnmsize;
int err;
int szamlalo=0;
char on=1;
char buffer[buff_size+1];
char buffer2[buff_size+1];
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons(port_no);
strcpy(buffer,argv[1]);
int meccs;

meccs=korok(buffer,meccs);

printf("%d\n",meccs);

fd = socket(AF_INET, SOCK_STREAM, 0);
if (fd<0){
	printf("%s Socket creation error\n", argv[0]);
	exit(1);
	}

setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

err=bind(fd, (struct sockaddr *) &server, server_size);
if (err<0){
	printf("%s: Cannot bind to the socket\n",argv[0]);
	exit(2);
}

err=listen(fd,10);
if (err < 0){
	printf("%s: Cannot listen to the socket\n",argv[0]);
}

printf("Várakozás az első kliensre\n");
fdc=accept(fd, (struct sockaddr *) & client, &client_size);
printf("Várakozás a második kliensre\n");
fdc2=accept(fd, (struct sockaddr *) & client2, &client_size2);
if(fdc < 0 || fdc2 < 0){
	printf("%s Cannot accept on socket\n", argv[0]);
}

printf("Pakli létrehozása\n");

FILE * befile=fopen("lapok","r");
char line[10];
int ertek;
int cnt=0;
srand((unsigned)time(NULL));

// pakli létrehozása 
while(!feof(befile)){
	fscanf(befile,"%s",line);
	strcpy(deck[c].name,line);
	fscanf(befile,"%s",line);
	strcpy(deck[c].type,line);
	fscanf(befile,"%d",&ertek);
	deck[c].value=ertek;
	fscanf(befile,"%d",&ertek);
	deck[c].status=ertek;
	c=c+1;
}
fclose(befile);



// POKER MECCS KEZDETE




while(szamlalo<meccs){
	//kéz kiosztás ügyelve hogy ne legyen ismétlés
	kioszt(deck,hand);
	kioszt(deck,hand2);

	// KÉT KIKÜLDÉS A KÉT KLIENSRE

	rendezes(hand);
	rendezes(hand2);

	for(int i=0;i<5;i++){
		sprintf(buffer,"%d %s(%s)\n",i+1,hand[i].name,hand[i].type);
		printf("%s\n",buffer);
		bytes=strlen(buffer)+1;
		printf("Kéz kiküldése a első kliensre %d Bájtok mennyisége:%d\n", i,bytes);
		trnmsize = send(fdc, buffer, bytes, 0);
   		if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		sprintf(buffer2,"%d %s(%s)\n",i+1,hand2[i].name,hand2[i].type);
		printf("%s\n",buffer2);
		bytes=strlen(buffer2)+1;
		printf("Kéz kiküldése a második kliensre %d bájtok mennyisége:%d\n", i,bytes);
		trnmsize = send(fdc2, buffer2, bytes, 0);
		if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
	}


	// CSERE SZÁMOK BEÉRKEZÉSE AZ ELSŐ MAJD A MÁSODIK KLIENSRŐL

	ertek=1;
	cnt=0;
	while(ertek!=0 && cnt<3){
		printf("Szám fogadása az első kliensről\n");
		recv(fdc,&ertek,bytes,0);
		cnt=cnt+1;
		if (ertek!=0){
			c=rand()%32;
			while(deck[c].status!=0)
				c=rand()%32;
			hand[ertek-1]=deck[c];
			deck[c].status=1;
			}
		}

	ertek=1;
	cnt=0;
	while(ertek!=0 && cnt<3){
		printf("szám fogadása a második kliensről\n");
		recv(fdc2,&ertek,bytes,0);
		cnt=cnt+1;
		if (ertek!=0){
			c=rand()%32;
			while(deck[c].status!=0)
				c=rand()%32;
			hand2[ertek-1]=deck[c];
			deck[c].status=1;
				}
		}

	rendezes(hand);
	rendezes(hand2);

	// Cserélt kéz visszaküldése

	for(int i=0;i<5;i=i+1){
		sprintf(buffer,"%d %s(%s)\n",i+1,hand[i].name,hand[i].type);
		bytes=strlen(buffer)+1;
		printf("%s",buffer);
		printf("Kéz kiküldése a első kliensre %d bájtok mennyisége:%d\n", i,bytes);
		trnmsize = send(fdc, buffer, bytes, 0);
   			if (trnmsize < 0) {
      			printf("%s: Cannot send data to the client.\n",argv[0]);
      			exit(6);
      			}
		sprintf(buffer2,"%d %s(%s)\n",i+1,hand2[i].name,hand2[i].type);
		bytes=strlen(buffer2)+1;
		printf("%s",buffer2);
		printf("Kéz kiküldése a második kliensre %d bájtok mennyisége:%d\n", i,bytes);
		trnmsize=send(fdc2, buffer2, bytes, 0);
		if (trnmsize < 0) {
      			printf("%s: Cannot send data to the client.\n",argv[0]);
      			exit(6);
      			}
		}

	

	printf("eredmény kiküldése a kliensekre");

	if (kiertekeles(hand)<kiertekeles(hand2)){
		strcpy(buffer,"Vesztettél! \n");
		bytes=strlen(buffer)+1;
		printf("Üzenet küldés az első kliensre\n");
		trnmsize=send(fdc, buffer, bytes, 0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		strcpy(buffer2,"Nyertél! \n");
		bytes=strlen(buffer2)+1;
		printf("Üzenet küldés a második kliensre:\n");
		trnmsize=send(fdc2, buffer2, bytes, 0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		}
	else if(kiertekeles(hand)>kiertekeles(hand2)){
		strcpy(buffer2,"Vesztettél! \n");
		bytes=strlen(buffer2)+1;
		printf("Üzenet küldés az első kliensre\n");
		trnmsize=send(fdc2, buffer2, bytes, 0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		strcpy(buffer,"Nyertél! \n");
		bytes=strlen(buffer)+1;
		printf("Üzenet küldés a második kliensre:\n");
		trnmsize=send(fdc, buffer, bytes, 0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		}
	else{
		strcpy(buffer,"Döntetlen! \n");
		bytes=strlen(buffer)+1;
		printf("Üzenet küldés az első kliensre\n");
		trnmsize=send(fdc, buffer, bytes, 0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		printf("Üzenet küldés a második kliensre:\n");
		trnmsize=send(fdc2, buffer, bytes, 0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		}


	printf("Pakli újrakeverése\n");
	for(int i=0;i<32;i++)
	deck[i].status=0;

	bytes=buff_size+1;
	recv(fdc,buffer,bytes,0);

	bytes=buff_size+1;
	recv(fdc2,buffer2,bytes,0);


	if (strcmp(buffer,"feladom")==0){
		strcpy(buffer2,"Nyertél,az ellenfeled feladta");
		bytes=strlen(buffer2);
		trnmsize=send(fdc2,buffer2,bytes,0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		strcpy(buffer,"Feladtad, vesztettél!");
		bytes=strlen(buffer);
		trnmsize=send(fdc,buffer,bytes,0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		close(fdc);
		close(fdc2);
		close(fd);
		exit(0);
		}

	if (strcmp(buffer2,"feladom")==0){
		strcpy(buffer,"Nyertél,az ellenfeled feladta");
		bytes=strlen(buffer);
		trnmsize=send(fdc,buffer,bytes,0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		strcpy(buffer2,"Vesztettél, feladtad");
		bytes=strlen(buffer2);
		trnmsize=send(fdc2,buffer2,bytes,0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		close(fdc);
		close(fdc2);
		close(fd);
		exit(0);
		}

	szamlalo++;

	if(szamlalo<meccs){
		strcpy(buffer,"Új kör kezdődik");
		printf("%s\n",buffer);
		bytes=strlen(buffer);
		trnmsize=send(fdc,buffer,bytes,0);	
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		trnmsize=send(fdc2,buffer,bytes,0);	
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
	}
	else{
		strcpy(buffer,"VÉGE");
		bytes=strlen(buffer);
		trnmsize=send(fdc,buffer,bytes,0);	
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
		trnmsize=send(fdc2,buffer,bytes,0);
			if (trnmsize < 0) {
      		printf("%s: Cannot send data to the client.\n",argv[0]);
      		exit(6);
      	}
	}

}


close(fdc);
close(fdc2);
close(fd);
exit(0);
}
