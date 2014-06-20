#ifndef PEOPLE_LIST_H
#define PEOPLE_LIST_H

#include "Bathroom.h"
#include "People.h"
#include <stdlib.h>
#include "parameters.h"


class PeopleList
{

	pthread_mutex_t m_data;
	PeopleData data;

	Bathroom::Type type;
	Bathroom *bathroom;

	
	pthread_t th_useBathroom;
	pthread_t th_makeInfos;
	pthread_t th_generatePerson;

	pthread_cond_t s_newPerson;


	pthread_mutex_t m_personsOnWait;
	pthread_mutex_t m_personsOnBathroom;
	pthread_mutex_t m_personsOutOfBathroom;


	list<Person*>  personsOnWait;
	list<Person*>  personsOnBathroom;
	list<Person*>  personsOutOfBathroom;

public:
	PeopleList(Bathroom *bathroom, Bathroom::Type tipo);
	PeopleData getData();
	void reopenLibert();


	friend void* thread_generatePerson(void *arg1);
	friend void* thread_useBathrooms(void *arg1);
	friend void* thread_makeInfos(void *arg1);

};

void* thread_generatePerson(void *arg1);
void* thread_useBathrooms(void *arg1);
void* thread_makeInfos(void *arg1);



#endif