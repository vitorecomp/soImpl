#include "Entities.h"



Problem::Problem(){

}

void Problem::solve(){
	system("echo 'execution' >> peopleThread.file");
	int waitTime = (rand() % (MAX_TIME_TO_USE_BATHROOM - MIN_TIME_TO_USE_BATHROOM)) + MIN_TIME_TO_USE_BATHROOM;
	usleep(waitTime*100);
	
}

PeopleData::PeopleData(){
	avarangeTimeN = 0;
	avarangeTimeEN = 0;
	avaranteTimeToExecution = 0;
	avaranteTimeOfExecution = 0;
}

void PeopleData::addExecutionAvarange(double time){
	avaranteTimeOfExecution = avaranteTimeOfExecution*avarangeTimeN + time;
	avarangeTimeN++;
	avaranteTimeOfExecution = avaranteTimeOfExecution/avarangeTimeN;

}

void PeopleData::addFinishedWaitAvarange(double time){
	avaranteTimeToExecution = avaranteTimeToExecution*avarangeTimeEN + time;
	avarangeTimeEN++;
	avaranteTimeToExecution = avaranteTimeToExecution/avarangeTimeEN;
}

void PeopleData::clearWaitingList(){
	while(!waitIds.empty())
		waitIds.pop_front();
}

void PeopleData::clearExecutionsList(){
	while(!executionsIds.empty())
		executionsIds.pop_front();
}

void PeopleData::addWaiting(int x, ulint y){
	waitIds.push_back(Tupla(x, y));
}		

PersonData::PersonData()
{
	executionTime = 0;
	waitingTime = 0;
	starttime = 0;
	totalWait = 0;
	inExecution = false;
	finished = false;
	onWait = false;
}