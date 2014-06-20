#ifndef BATHROOM_H
#define BATHROOM_H

#include <pthread.h>
#include <stdexcept>	
#include <list>
#include "parameters.h"

using namespace std;

class Bathroom
{
public:
	enum Type {Men, Women};
	static int numberOfTypes;
private:
	pthread_mutex_t m_changeType;
	pthread_mutex_t m_changeOcupedNumber;
	pthread_cond_t s_allFree;
	pthread_cond_t s_newFree;

	pthread_cond_t s_Free;
	pthread_cond_t s_changeType;
	

	int maxNumber;
	Type nowType;
	int ocuped;

public:

	Bathroom();
	~Bathroom();

	void freeOne() throw (length_error);
	void getOne(Type) throw (length_error, invalid_argument);
	void changeType(Type);
	void waitFreeSignal();
	void waitTypeSignal();

	//for scheduler
	Type getType();
};

#endif