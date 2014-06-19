#include <pthread.h>
#include "out.h"
#include "Bathroom.h"
#include "Entities.h"
#include "People.h"
#include "PeopleList.h"
#include "Scheduler.h"
#include <time.h>
#include <cstdlib>
#include <cstdio>

OUT out;

int main()
{
	srand(time(NULL));
	
	//inicializa as thread dos banheiros
	Bathroom bathroom;

	//incia a thread de criaçao de masculino
	PeopleList listM(&bathroom, Bathroom::Women);
	
	//start the thread of creation of femeale 
	PeopleList listW(&bathroom, Bathroom::Men);
	
	//colonacando em uma list
	list<PeopleList*> plist;
	plist.push_front(&listM);
	plist.push_front(&listW);


	//cria escalonador de dados
	Scheduler scheduler(plist , &bathroom);
	
	while(1){
		scheduler.run();
	}
}