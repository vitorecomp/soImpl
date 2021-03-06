#include "PeopleList.h"


PeopleList::PeopleList(Bathroom *bath, Bathroom::Type type)
{
	//save the arguments
	this->bathroom = bath;
	this->type = type;
	this->data.type = type;

	//start the class variables
	pthread_mutex_init(&m_personsOnWait, NULL);
	pthread_mutex_init(&m_personsOnBathroom, NULL);
	pthread_mutex_init(&m_personsOutOfBathroom, NULL);

	pthread_mutex_init(&m_data, NULL);
	pthread_cond_init (&s_newPerson, NULL);



	pthread_create(&th_generatePerson, NULL, thread_generatePerson, (void*)this);
	pthread_create(&th_useBathroom, NULL, thread_useBathrooms, (void*)this);
	pthread_create(&th_makeInfos, NULL, thread_makeInfos, (void*)this);
}


void* thread_generatePerson(void *arg1)
{
	PeopleList *plist = (PeopleList*)arg1;
	while(1)
	{ 
		pthread_mutex_lock(&plist->m_personsOnWait);
		if(plist->personsOnWait.size() < NUMBER_MAX_OF_THREADS_NOT_ALOCATES){	
			Person *person = new Person(plist->bathroom, plist->type);
			plist->personsOnWait.push_back(person);
			pthread_cond_signal(&plist->s_newPerson);
		}
		pthread_mutex_unlock (&plist->m_personsOnWait);

		int waitTime = rand() % (MAX_TIME_TO_SPAW - MIN_TIME_TO_SPAW) + MIN_TIME_TO_SPAW;
		usleep(waitTime*100);
	}
}

void* thread_useBathrooms(void *arg1)
{
	PeopleList *plist = (PeopleList*)arg1;
	while(1)
	{	
		try{			
			pthread_mutex_lock (&plist->m_personsOnBathroom);
			//verifico se na lista de execuntando quem ja terminou
			for (list<Person*>::iterator i=plist->personsOnBathroom.begin(); i != plist->personsOnBathroom.end(); ++i)
		    	if((*i)->terminei()){
					pthread_mutex_lock (&plist->m_personsOutOfBathroom);
					plist->personsOutOfBathroom.push_front(*i);
					pthread_mutex_unlock (&plist->m_personsOutOfBathroom);
		    		i = plist->personsOnBathroom.erase(i); 
		    	}

		    stringstream ss;
			ss << "echo 'em execuçao " << plist->personsOnBathroom.size()<< "' >> test.file";
			system(ss.str().c_str());
			pthread_mutex_unlock (&plist->m_personsOnBathroom);
			

			//peco um banheiro

			pthread_mutex_lock (&plist->m_personsOnWait);
			ss.str("");
			ss << "echo 'nao execution " << plist->personsOnWait.size()<< "' >> test.file";
			system(ss.str().c_str());
			if(plist->personsOnWait.empty())
				pthread_cond_wait(&plist->s_newPerson, &plist->m_personsOnWait);

			
			plist->bathroom->getOne(plist->type);

			//pego o primeiro da lista
			//movo ele para exevuntado
			
			Person *person = plist->personsOnWait.front();
			plist->personsOnWait.pop_front();
			pthread_mutex_unlock (&plist->m_personsOnWait);
			
			//colocando a pessoa para executar
			person->run();

			pthread_mutex_lock (&plist->m_personsOnBathroom);
			plist->personsOnBathroom.push_front(person);
			pthread_mutex_unlock (&plist->m_personsOnBathroom);
		} catch (const std::length_error& le) {
	  		if(le.what()[0] == '2'){
	  			pthread_mutex_unlock (&plist->m_personsOnWait);
				system("echo 'entrei aqui ' >> test.file");

	  			//exeption de falta de banhoiro
				//abro uma blocante experando banheiro
	  			plist->bathroom->waitFreeSignal();
	  		}
	  	} catch (const std::invalid_argument& le){

	  		if(le.what()[0] == '1'){
	  			pthread_mutex_unlock (&plist->m_personsOnWait);
	  			//exeption de tipo
				//abro uma blocante para a troca de tipo
	  			plist->bathroom->waitTypeSignal();
	  		}

  		}	
	}
}

void* thread_makeInfos(void *arg1)
{
	PeopleList *plist = (PeopleList*)arg1;
	while(1)
	{	
		//calulo das infos dos finalizados
		Person *person = NULL;
		pthread_mutex_lock (&plist->m_personsOutOfBathroom);
		if(!plist->personsOutOfBathroom.empty())
		{
			person = plist->personsOutOfBathroom.front();
			plist->personsOutOfBathroom.pop_front();
		}
		pthread_mutex_unlock (&plist->m_personsOutOfBathroom);

		if(person != NULL){

			PersonData pdata = person->getInfo();
			
			
			double execution = pdata.executionTime;
			double wait = pdata.waitingTime;
			delete person;

			pthread_mutex_lock (&plist->m_data);
			plist->data.addExecutionAvarange(execution);
			plist->data.addFinishedWaitAvarange(wait);
			pthread_mutex_unlock (&plist->m_data);
		}
		//calculos dos infos dos em execuçao
		pthread_mutex_lock (&plist->m_data);
		plist->data.clearExecutionsList();

		pthread_mutex_lock (&plist->m_personsOnBathroom);	
		for (list<Person*>::iterator i=plist->personsOnBathroom.begin(); i != plist->personsOnBathroom.end(); ++i){
			plist->data.executionsIds.push_back((*i)->getInfo().id);
    	}
		pthread_mutex_unlock (&plist->m_personsOnBathroom);
		pthread_mutex_unlock (&plist->m_data);

		pthread_mutex_lock (&plist->m_data);
		plist->data.clearWaitingList();

		pthread_mutex_lock (&plist->m_personsOnWait);	
		double wait = 0;
		for (list<Person*>::iterator i=plist->personsOnWait.begin(); i != plist->personsOnWait.end(); ++i){
			double wait = (*i)->getInfo().waitingTime;
			plist->data.waitIds.push_back(Tupla((*i)->getInfo().id, wait));
			plist->data.addFinishedWaitAvarange(wait);
    	}
		pthread_mutex_unlock (&plist->m_personsOnWait);
		pthread_mutex_unlock (&plist->m_data);
	}
}


PeopleData PeopleList::getData()
{
		pthread_mutex_lock (&m_data);
		PeopleData data = this->data;
		pthread_mutex_unlock (&m_data);
		

		return data;
}
