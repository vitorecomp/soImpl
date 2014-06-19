#include "Bathroom.h"

int Bathroom::numberOfTypes = 2;


Bathroom::Bathroom()
{
	this->maxNumber = 10;
	this->nowType = Women;
	this->ocuped = 0;
	pthread_mutex_init(&this->m_changeType, NULL);
	pthread_mutex_init(&this->m_changeOcupedNumber, NULL);
	pthread_cond_init (&this->s_allFree, NULL);
	pthread_cond_init (&this->s_newFree, NULL);
	pthread_cond_init (&this->s_Free, NULL);
	pthread_cond_init (&this->s_changeType, NULL);
}

Bathroom::~Bathroom()
{
	pthread_mutex_destroy(&this->m_changeOcupedNumber);
	pthread_mutex_destroy(&this->m_changeType);
	pthread_cond_destroy(&this->s_allFree);
	pthread_cond_destroy(&this->s_newFree);
	pthread_cond_destroy(&this->s_Free);
	pthread_cond_destroy(&this->s_changeType);
}

void Bathroom::freeOne() throw (length_error)
{
	pthread_mutex_lock (&m_changeOcupedNumber);
	if (ocuped < 1){
		pthread_mutex_unlock (&m_changeOcupedNumber);
		throw length_error("1 - Nao Existem Banheiros Ocupados");
	}
	ocuped--;
	if(ocuped == 0)
		pthread_cond_signal(&s_allFree);
	if(ocuped == 9)
		pthread_cond_signal(&s_newFree);

	pthread_cond_signal(&s_Free);
	pthread_mutex_unlock (&m_changeOcupedNumber);
}

void Bathroom::changeType(Type type)
{
	pthread_mutex_lock(&m_changeType);
	pthread_mutex_lock (&m_changeOcupedNumber);
	if(ocuped == 0)
	{
		this->nowType = type;
	}else{
		pthread_cond_wait(&s_allFree, &m_changeOcupedNumber);
		this->nowType = type;
	}

	pthread_cond_signal(&s_changeType);


	pthread_mutex_unlock (&m_changeOcupedNumber);
	pthread_mutex_unlock(&m_changeType);

}

void Bathroom::getOne(Type type) throw (length_error, invalid_argument)
{
	pthread_mutex_lock(&m_changeType);
	if(nowType != type){
		pthread_mutex_unlock (&m_changeOcupedNumber);
		throw invalid_argument("1 - Tipo diferente dos banheiros");
	}

	pthread_mutex_lock (&m_changeOcupedNumber);
	if(ocuped < 10)
	{
		ocuped++;
	}else{
		pthread_mutex_unlock (&m_changeOcupedNumber);
		pthread_mutex_unlock(&m_changeType);
		throw length_error("2 - Nao Existem Banheiros Livres");
	}
	pthread_mutex_unlock (&m_changeOcupedNumber);
	pthread_mutex_unlock(&m_changeType);
}

void Bathroom::waitFreeSignal()
{
	pthread_mutex_lock(&m_changeOcupedNumber);
	pthread_cond_wait(&s_Free, &m_changeOcupedNumber);
	pthread_mutex_unlock(&m_changeOcupedNumber);
}

void Bathroom::waitTypeSignal()
{	
	pthread_mutex_lock(&m_changeType);
	pthread_cond_wait(&s_changeType, &m_changeType);
	pthread_mutex_unlock(&m_changeType);
}

Bathroom::Type Bathroom::getType()
{
	pthread_mutex_lock(&m_changeType);
	Bathroom::Type type = this->nowType;
	pthread_mutex_unlock(&m_changeType);
	return type;	
}