#ifndef ENTITIES_H
#define ENTITIES_H

#define ulint unsigned long int

#include <list>
#include "Bathroom.h"

using namespace std;


class Tupla
{
	public:
		int x, y;
		Tupla(int x, int y)
		{
			this->x = x;
			this->y = y;
		}
};


class PersonData
{
public:
	ulint executionTime;
	ulint waitTime;
	ulint starttime;

};

class PeopleData
{
public:
	Bathroom::Type type;
	ulint avaranteTimeToExecution;
	ulint avaranteTimeOfExecution;
	list<int> executionsIds;
	list<Tupla> waitIds;

	void addExecutionAvarange(ulint);
	void addFinishedWaitAvarange(ulint);
	void clearWaitingList();
	void addWaiting(ulint);		
};

class BathData
{

};

class SchedulerData
{

};

class Problem
{
	int problemClass;
	int arg1, arg2, arg3;
public:
	Problem();
	void solve();
};

#endif
