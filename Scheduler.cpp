#include "Scheduler.h"

Scheduler::Scheduler(list<PeopleList*> lplist, Bathroom *bath)
{
	//passar ass variaveis
	while(!lplist.empty()){
		this->plist.push_front(lplist.front());
		lplist.pop_front();
	}
	this->bathroom = bath;
	maxE = 12;
	max = maxE / plist.size() - 1;
 
	int i = 2;
	//imprimir os titulos
	stringstream ss;
	ss << "Tipo Atual = ";
	out.message(ss.str(), i++, 2, 1);
	ss.str("");

	

	for(int x = 0; x < plist.size(); x++){
		ss << "Tempo espera de t" << x << "  =";
		out.message(ss.str(), i++, 2, 1);
		ss.str("");
	
		ss << "T medio exe de t" << x <<" =";
		out.message(ss.str(), i++, 2, 1);
		ss.str("");

		ss << "Tam fila de t" << x <<" =";
		out.message(ss.str(), i++, 2, 1);
		ss.str("");

		ss << "Pontuacao de t" << x <<" =";
		out.message(ss.str(), i++, 2, 1);
		ss.str("");

		i++;

	}
	

	ss << "Trocando =";
	out.message(ss.str(), i++, 2, 1);
	ss.str("");

}

void Scheduler::run()
{
	system("echo 'Scheduler in' >> sche.file");
	int i = 2;
	stringstream ss;
	//#error "fazer isso daqui"
	//ver o typo do banheiro
	Bathroom::Type type = bathroom->getType();
	system("echo 'Scheduler out' >> sche.file");

	//imprimir os titulos
	if(type == Bathroom::Women)
		ss << "Woman";
	else
		ss << "Man";

	out.message(ss.str(), i++, 15, 1);
	ss.str("");


	int j = 4, k = 2;
	for(list<PeopleList*>::iterator x = plist.begin(); x != plist.end(); ++x){
		PeopleData data = (*x)->getData();

		ss << data.avaranteTimeToExecution;
		out.message(ss.str(), i++, 22, 1);
		ss.str("");
	
		ss << data.avaranteTimeOfExecution;
		out.message(ss.str(), i++, 22, 1);
		ss.str("");

		out.runGraph(data.avaranteTimeToExecution, j++);

		ss << data.waitIds.size();
		out.message(ss.str(), i++, 22, 1);
		ss.str("");


		ss << 0;//calculusOfPontuation(data);
		out.message(ss.str(), i++, 22, 1);
		ss.str("");

		i++;
		usleep(100);
		
		if(data.type == type)
		{	
			system("echo '123 Terminei 1' >> sche.file");

			ss << "echo 'em execuçao " << data.executionsIds.size()<< "' >> test2.file";
			system(ss.str().c_str());
			ss.str("");
			int lin = 2;
			int imp = 0;

			for(list<int>::iterator p = data.executionsIds.begin(); p != data.executionsIds.end() && imp < maxE; p++, imp++){
				ss << "Theread " << *p << "         ";
				out.message(ss.str(), lin++, 1, 2);
				ss.str("");
			}
		}

		if(data.type == Bathroom::Women)
			ss << "Woman";
		else
			ss << "Man";
		out.message(ss.str(), k++, 1, 3);
		ss.str("");
		

		int imp = 0;
		for(list<Tupla>::iterator p = data.waitIds.begin(); p != data.waitIds.end() , imp < max; p++, imp++){
			if(p != data.waitIds.end()){
			ss << "Thr " << p->x << " wait " << p->y;
			}

			while(ss.str().length() < 23)
				ss << " ";
			out.message(ss.str(), k++, 1, 3);
			ss.str("");
		}
		k++;
		
	}

	ss << "nao";
	out.message(ss.str(), i++, 12, 1);
	ss.str("");

	//ver se compença mudar de banheiros
	if(changeType()){
		ss << "sim";
		out.message(ss.str(), i++, 12, 1);
		ss.str("");
		if(bathroom->getType() == Bathroom::Women)
			bathroom->changeType(Bathroom::Men);
		else
			bathroom->changeType(Bathroom::Women);

	}	
}

int Scheduler::calculusOfPontuation(PeopleData data){
	Bathroom::Type type = bathroom->getType();
	int wait_list = data.waitIds.size();
	int i=0;
	int pontuation;
	for(list<Tupla>::iterator i = data.waitIds.begin(); i != data.waitIds.end(); ++i){
		pontuation+=pow(i->y,2);
	}
	return pontuation;
}

bool Scheduler::changeType(){
	int pont_Masc = calculusOfPontuation((plist.front())->getData());
	int pont_Fem = calculusOfPontuation((plist.back())->getData());
	Bathroom::Type type = bathroom->getType();
	if(type==Bathroom::Men && pont_Fem>pont_Masc && pont_Fem>PONT_MIN_LINE_PEOPLE){
		return true;
	}else if(type==Bathroom::Women && pont_Masc>pont_Fem && pont_Masc>PONT_MIN_LINE_PEOPLE){
		return true;
	}
	return false;
}


