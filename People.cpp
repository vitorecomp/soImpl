#include "People.h"

Person::Person(Bathroom *bath)
{
	this->bath = bath;
	pthread_mutex_init(&finish, NULL);
	pthread_mutex_init(&start_c, NULL);
	pthread_cond_init (&start, NULL);
    gettimeofday(&start_time , NULL);
    b_terminei = false;
   	
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
	pthread_cond_signal(&start);
	pthread_mutex_unlock (&start_c);
}


void Person::execute(){
	pthread_mutex_lock (&start_c);
	pthread_cond_wait(&start, &start_c);
	pthread_mutex_unlock (&start_c);
    
    //make the processing
    gettimeofday(&processing_start , NULL);
	Problem problem;
	problem.solve(); 
    gettimeofday(&processing_end , NULL);

    pthread_mutex_lock (&finish);
	b_terminei = true;
	pthread_mutex_unlock (&finish);
}


void* thread_person(void *arg1)
{
	Person *person = (Person*)arg1;
 	person->execute();
 	return person;
}