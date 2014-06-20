#ifndef PEOPLE_H
#define PEOPLE_H

#include "Bathroom.h"
#include <sys/time.h>
#include "Entities.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include <pthread.h>

using namespace std;


class Person
{
	static int unicId;
	int id;
	Bathroom *bath;
	PersonData data;
	pthread_t m_threads;
	bool liberado;
	pthread_mutex_t finish;
	pthread_mutex_t m_run;
	pthread_mutex_t start_c;
	pthread_cond_t start;

	bool b_terminei;
	struct timeval start_time, processing_start,  processing_end;

	
public:
	void execute();
	Person(Bathroom *);
	~Person();
	bool terminei();
	void run();
	PersonData getInfo();
};


void* thread_person(void *arg1);

#endif