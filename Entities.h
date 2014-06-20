#ifndef ENTITIES_H
#define ENTITIES_H

#define ulint unsigned long int

#include <list>
#include "Bathroom.h"
#include <cstdlib>
#include "parameters.h"

using namespace std;


class Tupla
{
	public:
		int x; 
		double y;
		Tupla(int x, double y)
		{
			this->x = x;
			this->y = y;
		}
};


class PersonData
{
public:
	int id;
	bool inExecution;
	bool finished;
	bool onWait;
	double executionTime;
	double waitingTime;
	double starttime;
	double totalWait;
	PersonData();

};

class PeopleData
{
	ulint avarangeTimeN;
	ulint avarangeTimeEN;
public:
	Bathroom::Type type;
	double avaranteTimeToExecution;
	double avaranteTimeOfExecution;
	list<int> executionsIds;
	list<Tupla> waitIds;

	void addExecutionAvarange(double);
	void addFinishedWaitAvarange(double);
	void clearWaitingList();
	void clearExecutionsList();
	void addWaiting(int, ulint);	
	PeopleData();	
};

class Problem
{
	int problemClass;
	int arg1, arg2, arg3;
public:
	Problem();
	void solve();
};

double time_diff(struct timeval x , struct timeval y);

inline double time_diff(struct timeval y , struct timeval x)
{
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
     
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}

#endif
