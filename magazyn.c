//komunikacja magazyn <-> kurier
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
#include <fcntl.h>
#include <sys/stat.h>

//szkielet wiadomości magazyn<-> dystrybutornia + magazyn <-> kurier
typedef struct Message
{
	long mtyp;
	int zamA;
	int zamB;
	int zamC;
}Message;

//zawartość magazynu + ceny
struct Magazyn{
int a;
int b;
int c;
int cenaA;
int cenaB;
int cenaC;
};

//tablica kurierów dla naszego magazynu
typedef struct{
	int id;
	int mamona;
}TabKurier;

//działanie kuriera
void kurier(TabKurier *kurier, int kolejka_id,int klucz){


struct Message prosba, odpowiedz;
//odbieramy wiadomość
Message msg;
int buf = msgget(klucz, 0777);
//odbieranie wiadomości ma być wykonywane przez kurierów!
int i = msgrcv(buf, &msg, 6, 1,0);
printf("%d zamowienie a\n", msg.zamA);

//najpierw prosimy
prosba.mtyp = 66; //klucz dla kurier - magazyn
prosba.zamA = msg.zamA;
prosba.zamB = msg.zamB;
prosba.zamC = msg.zamC;

msgsnd(kolejka_id, &prosba, sizeof(struct Message) - sizeof(long),0);

//teraz czekamy na odpowiedz
msgrcv(kolejka_id, &odpowiedz, sizeof(struct Message) - sizeof(long), 0, 0);
//tu sprawdzimy czy w magazynie jest wystarczająco produktów jesli za mało kurier sie zabija
if(prosba.zamA>=odpowiedz.zamA || prosba.zamB>=odpowiedz.zamB || prosba.zamC>=odpowiedz.zamC){
	printf("%d kurier\n", kurier->id);// czy to tak ma być?
	//tu ma się zabic ale jak?
	//break;
	}




}


int main(int argc, char *argv[]){
//ten klucz mamy pobrać z terminala
int klucz = 0x32;
//dane magazynu, je odczytujemy z pliku
struct Magazyn magazyn ={200, 300, 200, 5, 6, 7};

//tworzymy tablicę kurierów
TabKurier tabkurier[3];
//tworzymy kolejkę tego magazynu
int kluczmagazyn = 0x88;
int kolejka_id = msgget(kluczmagazyn, IPC_CREAT |0777);

//procesy kurierów
for (int i =0; i<3;i++){
tabkurier[i].id = i+1;
tabkurier[i].mamona = 0;
if (fork()==0){
kurier(&tabkurier[i], kolejka_id, klucz);
}
}



return 0;}
