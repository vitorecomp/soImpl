#include "People.h"

int Person::unicId = 0;

Person::Person(Bathroom *bath, int Iden)
{
	id = (Iden + 1)*10000 + unicId++;
	this->bath = bath;
	pthread_mutex_init(&finish, NULL);
	pthread_mutex_init(&start_c, NULL);
	pthread_cond_init (&start, NULL);
	pthread_mutex_init(&m_run, NULL);

    pthread_mutex_lock (&finish);
    gettimeofday(&start_time , NULL);
    b_terminei = false;
	data.onWait = true;
	liberado = false;
	data.id = id;
	
	pthread_mutex_unlock (&finish);
	

   	
   	//start na thread
	pthread_create(&m_threads, NULL, thread_person, (void*)this);
}

bool Person::terminei()
{
	bool myterminei;
	pthread_mutex_lock (&finish);
	myterminei = b_terminei;
	pthread_mutex_unlock (&finish);
	return myterminei;


}

Person::~Person()
{
	pthread_mutex_destroy(&finish);
	pthread_mutex_destroy(&start_c);
	pthread_cond_destroy(&start);
}

void Person::run()
{	
	pthread_mutex_lock (&start_c);
	liberado = true;
	pthread_cond_signal(&start);
	pthread_mutex_unlock (&start_c);
}


void Person::execute(){
	pthread_mutex_lock (&start_c);
	if(!liberado)
		pthread_cond_wait(&start, &start_c);
	pthread_mutex_unlock (&start_c);
    

	pthread_mutex_lock (&finish);
	//setar o wait time
	struct timeval actual_time;
    gettimeofday(&actual_time , NULL);
	data.totalWait = time_diff(actual_time , start_time);

	//falar que estou exxecutando
	data.onWait = false;
	data.inExecution = true;
	pthread_mutex_unlock (&finish);
	

    //make the processing
    gettimeofday(&processing_start , NULL);
	Problem problem;
	problem.solve(); 
    gettimeofday(&processing_end , NULL);


    
	pthread_mutex_lock (&finish);
	//setar o tempo de porcessamento
	data.executionTime = time_diff(processing_end , processing_start);

	//falar que estou exxecutando
	data.inExecution = false;
	data.finished = true;
	b_terminei = true;
	pthread_mutex_unlock (&finish);
	
	bath->freeOne();

}

PersonData Person::getInfo()
{
	pthread_mutex_lock (&finish);

	if(data.onWait){
		//setar o wait time
		struct timeval actual_time;
	    gettimeofday(&actual_time , NULL);
		data.waitingTime = time_diff(actual_time , start_time);
	}
	PersonData pdata = data;
	pthread_mutex_unlock (&finish);
	return pdata;
}

void* thread_person(void *arg1)
{
	Person *person = (Person*)arg1;
 	person->execute();
}