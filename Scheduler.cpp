#include "Scheduler.h"

Scheduler::Scheduler(list<PeopleList*> lplist, Bathroom *bath)
{
	//passar ass variaveis
	this->plist = lplist;
	this->bathroom = bath;

	int i = 0;
	//imprimir os titulos
	stringstream ss;
	ss << "Tipo Atual = ";
	out.message(ss.str(), i++, 1, 1);
	ss.clear();

	i++;

	for(int x = 0; x < lplist.size(); x++){
		ss << "Tempo espera de t" << x << "  =";
		out.message(ss.str(), i++, 1, 1);
		ss.clear();
	
		ss << "T medio exe de t" << x <<" =";
		out.message(ss.str(), i++, 1, 1);
		ss.clear();

		ss << "Tam fila de t" << x <<" =";
		out.message(ss.str(), i++, 1, 1);
		ss.clear();

		ss << "Pontuacao de t" << x <<" =";
		out.message(ss.str(), i++, 1, 1);
		ss.clear();

		i++;

	}

	ss << "Trocando =";
	out.message(ss.str(), i++, 1, 1);
	ss.clear();

}

void Scheduler::run()
{
	int i = 0;
	stringstream ss;
	//#error "fazer isso daqui"
	//ver o typo do banheiro
	Bathroom::Type type = bathroom->getType();

	//imprimir os titulos
	if(type == Bathroom::Women)
		ss << "Woman";
	else
		ss << "Man";

	out.message(ss.str(), i++, 14, 1);
	ss.clear();

	i++;

	int j = 4, k = 0;
	for(list<PeopleList*>::iterator x = plist.begin(); x != plist.end(); x++){
		PeopleData data = (*x)->getData();
		ss << data.avaranteTimeToExecution;
		out.message(ss.str(), i++, 22, 1);
		ss.clear();
	
		ss << data.avaranteTimeOfExecution;
		out.message(ss.str(), i++, 22, 1);
		ss.clear();

		out.runGraph(data.avaranteTimeToExecution, j++);

		ss << data.waitIds.size();
		out.message(ss.str(), i++, 22, 1);
		ss.clear();


		ss << calculusOfPontuation(data);
		out.message(ss.str(), i++, 22, 1);
		ss.clear();

		i++;

		if(data.type == type)
		{
			int lin = 1;
			int imp = 0;
			list<int> ids = data.executionsIds;
			for(list<int>::iterator p = ids.begin(); p != ids.end(), imp < maxE; p++, imp++){
				ss << "Theread " << *p;
				out.message(ss.str(), lin++, 1, 2);
				ss.clear();
			}
		}

		list<Tupla> ids = data.waitIds;
		int imp = 0;
		for(list<Tupla>::iterator p = ids.begin(); p != ids.end(), imp < max; p++, imp++){
			ss << "Theread " << p->x << " wait " << p->y;
			out.message(ss.str(), k++, 1, 2);
			ss.clear();
		}
		k++;
	}

	ss << "nao";
	out.message(ss.str(), i++, 12, 1);
	ss.clear();


	//ver se comṕença mudar de banheiros
	if(changeType()){
		ss << "sim";
		out.message(ss.str(), i++, 12, 1);
		ss.clear();
		bathroom->changeType(this->highScore());
	}
}
