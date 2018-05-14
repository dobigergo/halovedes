#include <stdlib.h>
#include <stdio.h>

struct card{
char name[10];
char type[5];
int value;
int status;
};

int par(struct card valami[5]){
int ertek=0;
for(int i=0;i<5;i=i+1)
 for(int j=i+1;j<5;j=j+1)
	if(strcmp(valami[i].name,valami[j].name)==0 && valami[i].value > ertek)
	ertek= valami[i].value;
return ertek;
}

int drill(struct card valami[5]){
int ertek=0;
for(int i=0;i<5;i=i+1)
 for(int j=i+1;j<5;j=j+1)
  for(int k=j+1;k<5;k=k+1)
	if(strcmp(valami[i].name,valami[j].name)==0 && strcmp(valami[i].name,valami[k].name)==0)
	ertek= valami[i].value;
return ertek;
}

int duplapar(struct card valami[5]){
int ertek=0;
for(int i=0;i<5;i=i+1)
 for(int j=i+1;j<5;j=j+1)
	if(strcmp(valami[i].name,valami[j].name)==0 && valami[i].value != par(valami))
	ertek=par(valami);
return ertek;
}

int poker(struct card valami[5]){
int ertek=0;
 for(int i=0;i<2;i=i+1)
    if(strcmp(valami[i].name,valami[i+1].name)==0 && strcmp(valami[i].name,valami[i+2].name)==0 && strcmp(valami[i].name,valami[i+3].name)==0 )
     ertek=valami[i].value;
return ertek;
}

struct card* rendezes(struct card valami[5]){
int ertek=0;
int min;
struct card kulcs;
for (int i=0;i<4;i++){
min=i;
	for(int j=i+1;j<5;j++)
		if(valami[min].value>valami[j].value)
			min=j;
 kulcs=valami[min];
 valami[min]=valami[i];
 valami[i]=kulcs;

}
return valami;

}

int sor(struct card valami[5]){
int ertek=0;
if(valami[0].value==valami[1].value-1 && valami[1].value==valami[2].value-1 && valami[2].value==valami[3].value-1 && valami[3].value==valami[4].value-1)
ertek=valami[4].value;
return ertek;

}

int flush(struct card valami[5]){
int ertek=0;
if(strcmp(valami[0].type,valami[1].type)==0 && strcmp(valami[0].type,valami[2].type)==0 && strcmp(valami[0].type,valami[3].type)==0 && strcmp(valami[0].type,valami[4].type)==0)
ertek=valami[4].value;
return ertek;
}

int full(struct card valami[5]){
int ertek=0;
for(int i=0;i<5;i=i+1)
 for(int j=i+1;j<5;j=j+1)
	if(strcmp(valami[i].name,valami[j].name)==0 && drill(valami)!=0 && valami[i].value != drill(valami))
	ertek=par(valami);
return ertek;
}

int szinsor(struct card valami[5]){
int ertek=0;
if(sor(valami)!=0 && flush(valami)!=0 )
ertek=sor(valami);
return ertek;
}

int royal_flush(struct card valami[5]){
int ertek=0;
if(szinsor(valami)!=0 && valami[0].value==4)
ertek=sor(valami);
return ertek;
}

int kiertekeles(struct card hand[5]){
printf("\n");
if(royal_flush(hand)){
printf("van egy royal flösöd\n");
return 10;}
else if(szinsor(hand)){
printf("van egy szín sorod\n");
return 9;}
else if(poker(hand)){
printf("van egy pokered\n");
return 8;}
else if(full(hand)){
printf("van egy fullod\n");
return 7;}
else if(flush(hand)){
printf("van egy flöshöd\n");
return 6;}
else if(sor(hand)){
printf("van egy sorod\n");
return 5;}
else if(drill(hand)){
printf("van egy drilled\n");
return 4;}
else if (duplapar(hand)){
printf("van egy dupla párod\n");
return 3;}
else if (par(hand)){
printf("van egy párod\n");
return 2;}
else{
printf("nincs semmid lol\n");
return 1;}
}

void kioszt(struct card deck[30], struct card hand[5]){
int c=0;
for (int i=0;i<5;i=i+1){
c=rand()%32;
while(deck[c].status!=0)
c=rand()%32;
if (deck[c].status == 0){
hand[i]=deck[c];
deck[c].status=1;
}
}
}

int korok(char * meccs, int szam){
if(strcmp(meccs,"egy")==0){
szam=1;
return szam;
}
else if(strcmp(meccs,"kettő")==0){
szam=2;
return szam;
}
else if(strcmp(meccs,"három")==0){
szam=3;
return szam;
}
else if(strcmp(meccs,"négy")==0){
szam=4;
return szam;
}
else if(strcmp(meccs,"öt")==0){
szam=5;
return szam;
}
szam=3;
return szam;
}


