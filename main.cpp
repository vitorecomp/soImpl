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
#include <csignal>

OUT out;

void signalHandler( int signum )
{
    cout << "Interrupt signal (" << signum << ") received.\n";

    // cleanup and close up stuff here  
    // terminate program  

   exit(signum);  

}


int main()
{
	signal(	SIGABRT, signalHandler); 
	srand(time(NULL));
	system("rm peopleThread.file");
	system("rm sche.file");
	system("rm test.file");
	system("rm test2.file");
	//inicializa as thread dos banheiros
	Bathroom bathroom;

	//incia a thread de criaçao de femino
	PeopleList listW(&bathroom, Bathroom::Women);
	
	//start the thread of creation of males 
	PeopleList listM(&bathroom, Bathroom::Men);
	
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