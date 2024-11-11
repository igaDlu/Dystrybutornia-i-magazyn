#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/msg.h>

typedef struct Message
{
	long mtyp;
	int zamA;
	int zamB;
	int zamC;
}Message;

typedef struct KOSZT
{
long mtyp;
int mkoszt;
}KOSZT;


typedef struct{
	int A;//aktualne zapotrzebowanie na A
	int B;// -,,- B
	int C;
}Dystrybutornia; // to nie potzrebne 

//=====LOSOWANIE -> odbywa się za pomocą funkcji rand(), uwzględniając maksimum pobrane z wejścia terminala
void losowanie(Dystrybutornia *dystrybutornia, int X, int Y, int Z, int *a, int *b, int *c){
		usleep(500000);
		
		
		int A = rand()%X + 1;//losowanie z maxA
		int B = rand()%Y + 1;//losowanie z maxB
		int C = rand()%Z + 1;//losowanie z maxC
		*a = A;
		*b = B;
		*c = C;
	}


int main(int argc, char *argv[]){
if (argc <5){
	printf("podano za mało argumentów w poleceniu, podaj: numer kolejki, liczbę zamówień, max A, max B i max C");}
else{
Dystrybutornia *dystrybutornia;//to nam niepotrzebne
//konstrukcja wiadomości
int buf;
key_t klucz = atoi(argv[1]); //klucz pobierany z terminala
Message msg;
buf = msgget(klucz, IPC_CREAT|0777);
msgctl(buf, IPC_RMID,0);
buf = msgget(klucz, IPC_CREAT|0777);
msg.mtyp = 1; //typ możemy zmienić??

int a, b, c;
int iloscZamowien = atoi(argv[2]);
int maxA = atoi(argv[3]);
int maxB = atoi(argv[4]);
int maxC = atoi(argv[5]);

for (int i =0; i<iloscZamowien;i++){
losowanie(dystrybutornia, maxA, maxB, maxC,&a, &b, &c);
//nadajemy wartości intom w kolejce komunikatów

msg.zamA = a;
msg.zamB = b;
msg.zamC = c;
//wysyłamy wiadomość
msgsnd(buf, &msg,sizeof(Message) -sizeof(long),0); 

printf("%d %d %d wartość a, b, c\n", a, b, c);}


}

//=====Odbieramy sumę od magazynów
printf("czekamy na całkowity koszt zleceń od magazynów\n");
int m=0;
int z;
int KOSZTCALKOWITY = 0;
key_t kluczZ = 0x55;
KOSZT koszt;

z = msgget(kluczZ,0777);
while(1){
int i = msgrcv(z,&koszt, 6,1,0);
if(i>0 && m<2){
printf("odebrano wiadomosc zawierajaca: %d\n", koszt.mkoszt);
KOSZTCALKOWITY+=koszt.mkoszt;
m++;
}
else{
if(m>=1){
printf("odebrano wiadomosc zawierajaca: %d\n", koszt.mkoszt);
KOSZTCALKOWITY+=koszt.mkoszt;
break;}
}
}
printf("\n całkowita zapłata za wszystkie zlecenia to %d GLD \n", KOSZTCALKOWITY);
//msgctl(buf, IPC_RMID,0);

return 0;
}
