#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Bathroom.h"
#include "People.h"
#include "PeopleList.h"
#include <pthread.h>
#include <list>
#include <string>
#include <sstream>
#include "out.h"

using namespace std;

extern OUT out;

class Scheduler
{
	int maxE, max;
 	list<PeopleList*> plist;
 	Bathroom *bathroom;

 	int calculusOfPontuation(PeopleData data);
 	bool changeType();
 	Bathroom::Type highScore();
 public:
 	Scheduler(list<PeopleList*>, Bathroom*);
 	void run();

};

#endif