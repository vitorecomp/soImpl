#include "out.h"

OUT::OUT(){
	size.x =45;
	size.y =100;
	int i = system("resize -s 45 100");
	i += system("clear");
	if(i > 100000)
		i++;

	c_box(OutPoint(0, 0), OutPoint(15, 40), "System Control", 1);
	c_box(OutPoint(0, 41), OutPoint(15, 70), " \u2642 Running Threads ", 2);
	c_box(OutPoint(0, 71), OutPoint(15, 100), " \u2640 Wating Threads ", 3);


	BarGInfo info;
	info.id = 4;
	info.uple = OutPoint(16, 0);
	info.bori = OutPoint(22, 100);
	info.max = 2000000;
	info.min = 0;
	info.legend = "Wait time[us] = ";
	info.description = "Men avarenge wait time";
	c_barGraph(info);
	
	info.id = 5;
	info.uple = OutPoint(23, 0);
	info.bori = OutPoint(29, 100);
	info.max = 2000000;
	info.min = 0;
	info.legend = "Wait time[us] = ";
	info.description = "Women avarenge wait time";
	c_barGraph(info);

	cout << flush;


	pthread_mutex_init(&listLock, NULL);
	pthread_mutex_init(&infoListsLock, NULL);
	pthread_cond_init (&restartPrint, NULL);

	pthread_create(&out_threads, NULL, thread_out, (void*)this);

}

void OUT::c_barGraph(BarGInfo info){
	d_box(info.uple, info.bori);

	changeColor(Blue);
	move(info.uple.x, info.uple.y + 3);
		cout << info.description;

	changeColor(Default);
	move(info.uple.x + 2, info.uple.y + 3);
		cout << info.legend << "1000000 us";

	changeColor(Yellow);
	move(info.uple.x + 3, info.uple.y + 3);
		for(int i = info.uple.y + 2; i < (info.bori.y - info.uple.y + 2)/2; i++)
			cout << "=";

	changeColor(Default);
	cout << flush;

	pthread_mutex_lock (&infoListsLock);
	barlist.push_front(info);
	pthread_mutex_unlock (&infoListsLock);
}

void OUT::c_box(OutPoint se, OutPoint id, string legenda, int iden){
	d_box(se, id);
	changeColor(Blue);
	move(se.x, se.y + 3);
		cout << legenda;

	changeColor(Default);

	cout << flush;

	BoxInfo info;
	info.uple = se;
	info.bori = id;
	info.title = legenda;
	info.id = iden;


	pthread_mutex_lock (&infoListsLock);
	boxlist.push_front(info);
	pthread_mutex_unlock (&infoListsLock);

}

void OUT::d_box(OutPoint se, OutPoint id){
	move(se.x, se.y);
	for(int i = se.y; i< id.y; i++)
		cout << "\u2500";


	move(id.x, se.y);
	for(int i = se.y; i< id.y; i++)
		cout << "\u2500";

	move(se.x, se.y);
	for(int i = se.x; i< id.x; i++){
		move(i, se.y);
		cout << "\u2502";
	}

	move(se.x, id.y);
	for(int i = se.x; i< id.x; i++){
		move(i, id.y);
		cout << "\u2502";
	}

	move(se.x, se.y);
	cout << "\u250C";

	move(se.x, id.y);
	cout << "\u2510";

	move(id.x, se.y);
	cout << "\u2514";

	move(id.x, id.y);
	cout << "\u2518";


	cout << flush;
}

void OUT::c_graph(GraphInfo para){
	/*
	//print bot name
	int startBotname = ((para.fimPoint.y - para.iniPoint.y) - para.ylegend.length())/2 + para.iniPoint.y;
	move(para.fimPoint.x -1, startBotname);
	cout << para.ylegend;
	
	//print size name
	int startLeftName = ((para.fimPoint.x - para.iniPoint.x) - para.xlegend.length())/2 + para.iniPoint.x;
	move(startLeftName, para.iniPoint.y + 2);
	for(int i =0; i < para.xlegend.length(); i++)
	{
		move(startLeftName + i, para.iniPoint.y + 3);
		cout << para.xlegend.at(i);
	}


	//print bot line



	//print size line


	//print pesoudo cols

	//print peseudo lines

	cout << flush;
	*/
}

OutPoint::OutPoint(int x, int y){
	this->x = x;
	this->y = y;
}

OutPoint::OutPoint(){
	this->x = 0;
	this->y = 0;
}

void OUT::changeColor(Color color){
	if(color == Default)
		cout << "\e[39m";

	if(color == Black)
		cout << "\e[30m";

	if(color == Red)
		cout << "\e[31m";

	if(color == Green)
		cout << "\e[32m";

	if(color == Yellow)
		cout << "\e[33m";

	if(color == Blue)
		cout << "\e[34m";

	if(color == Magentta)
		cout << "\e[35m";

	if(color == Cyan)
		cout << "\e[36m";

	if(color == LGray)
		cout << "\e[37m";

	if(color == DGray)
		cout << "\e[90m";

	if(color == LRed)
		cout << "\e[91m";

	if(color == LGreen)
		cout << "\e[92m";

	if(color == LYellow)
		cout << "\e[93m";

	if(color == LBlue)
		cout << "\e[94m";

	if(color == LMagentta)
		cout << "\e[95m";

	if(color == LCyan)
		cout << "\e[96m";

	if(color == White)
		cout << "\e[97m";
}

void OUT::message(string msg, int offx, int offy) throw (invalid_argument){
	if(offx + msg.length()> size.x)
		throw invalid_argument("menssagem fora dos limites da tela 'x'");


	if(offy > size.y)
		throw invalid_argument("menssagem fora dos limites da tela 'y'");

	OutCommand command(offx, offy, msg, Default);


	pthread_mutex_lock (&listLock);
	commands.push_front(command);	
	pthread_mutex_unlock (&listLock);
}

void OUT::message(string msg, int offx, int offy, Color color) throw (invalid_argument){

	if(offx + msg.length() > size.x)
		throw invalid_argument("menssagem fora dos limites da tela 'x'");


	if(offy > size.y)
		throw invalid_argument("menssagem fora dos limites da tela 'y'");

	OutCommand command(offx, offy, msg, color);
	pthread_mutex_lock (&listLock);
	commands.push_front(command);	
	pthread_mutex_unlock (&listLock);
}

void OUT::message(string msg, int offx, int offy, int id) throw (invalid_argument){
	OutPoint uple, bori, size;
	pthread_mutex_lock (&infoListsLock);
		for (list<BoxInfo>::iterator i=boxlist.begin(); i != boxlist.end(); ++i)
	    	if(i->id == id){
	    		uple = i->uple;
	    		bori = i->bori; 
	    	}
    pthread_mutex_unlock (&infoListsLock);
    size.x = bori.x - uple.x;
    size.y = bori.y - uple.y;


	if(offx + msg.length()> size.x)
		throw invalid_argument("menssagem fora dos limites da tela 'x'");


	if(offy > size.y)
		throw invalid_argument("menssagem fora dos limites da tela 'y'");


	OutCommand command(offx + uple.x, offy + uple.y, msg, Default);

	pthread_mutex_lock (&listLock);
	commands.push_front(command);	
	pthread_mutex_unlock (&listLock);
}

void OUT::message(string msg, int offx, int offy, int id, Color color) throw (invalid_argument){

	OutPoint uple, bori, size;
	pthread_mutex_lock (&infoListsLock);
		for (list<BoxInfo>::iterator i=boxlist.begin(); i != boxlist.end(); ++i)
	    	if(i->id == id){
	    		uple = i->uple;
	    		bori = i->bori; 
	    	}
    pthread_mutex_unlock (&infoListsLock);
    size.x = bori.x - uple.x;
    size.y = bori.y - uple.y;


	if(offx + msg.length()> size.x)
		throw invalid_argument("menssagem fora dos limites da tela 'x'");


	if(offy > size.y)
		throw invalid_argument("menssagem fora dos limites da tela 'y'");


	OutCommand command(offx + uple.x, offy + uple.y, msg, color);

	pthread_mutex_lock (&listLock);
	commands.push_front(command);	
	pthread_mutex_unlock (&listLock);
}

OutCommand::OutCommand(int y, int x, string msg, OUT::Color color){
	this->x = x;
	this->y = y;
	this->msg = msg;
	this->color = color;
}

void* thread_out(void *arg1){
	OUT *out = (OUT*)arg1;
 	out->run();
 	pthread_exit(NULL);
}

void OUT::run(){
	move(14, 2);
	cout <<  "Out list size = "<< commands.size() << "  ";
	OutCommand out;
	bool has = false;
	
	pthread_mutex_lock (&listLock);
	if(!commands.empty()){
		has = true;
		out = commands.front();
		commands.pop_front();
	}
	pthread_mutex_unlock (&listLock);	

	if(has){
		move(out.x, out.y);
		changeColor(out.color);
		cout << out.msg;
		changeColor(Default);

	}
	
	cout << flush;

}

void OUT::runGraph(int value, int id)
{
	BarGInfo info;
	pthread_mutex_lock (&infoListsLock);
		for (list<BarGInfo>::iterator i=barlist.begin(); i != barlist.end(); ++i)
	    	if(i->id == id){
	    		info = *i; 
	    	}
    pthread_mutex_unlock (&infoListsLock);

	changeColor(Default);
	move(info.uple.x + 2, info.uple.y + 3);
		cout << info.legend << value << " us";

	stringstream ss;
	Color color;
	int porcentagem = (max - min)/value;
	int barras = porcentagem * (info.bori.y - info.uple.y);
	if(porcentagem < 0.33)
		color = Green;
	else if (porcentagem < 0.66)
		color = Yellow;
	else 
		color = Red;

	for(int i = info.uple.y + 2; i < (info.bori.y - info.uple.y + 2)/2; i++)
		ss << "=";

	OutCommand command(info.uple.x + 3, info.uple.y + 3, ss.str(), Red);

	pthread_mutex_lock (&listLock);
	commands.push_front(command);	
	pthread_mutex_unlock (&listLock);

}