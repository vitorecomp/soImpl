#ifndef OUT_H
#define OUT_H

#include <iostream>
#include <list>
#include <cstdlib>	
#include <stdexcept>
#include <string>
#include <sstream>
#include <pthread.h>

using namespace std;


class OutPoint
{
public:
	int x;
	int y;
	OutPoint(int x, int y);
	OutPoint();
};

class GraphInfo;
class BarGInfo;
class BoxInfo;
class OutCommand;

void* thread_out(void *arg1);


class OUT
{

public:
	enum Color{Default, Black, Red, Green, Yellow, Blue, Magentta, Cyan, LGray, DGray, LRed ,LGreen ,LYellow, LBlue, LMagentta, LCyan, White};

private:
	OutPoint size;
	pthread_mutex_t listLock;
	pthread_cond_t restartPrint;
	list<OutCommand> commands;

	pthread_mutex_t infoListsLock;
	list<BarGInfo> barlist;
	list<GraphInfo> graphlist;
	list<BoxInfo> boxlist;

	pthread_t out_threads;

	

	void move(int x, int y);
	void changeColor(OUT::Color);

	void d_box(OutPoint, OutPoint);


	void c_box(OutPoint, OutPoint, string, int);
	void c_graph(GraphInfo);
	void c_barGraph(BarGInfo);
	
public:
	void run();	
	OUT();

	void message(string msg, int offx, int offy) throw (invalid_argument);
	void message(string msg, int offx, int offy, Color) throw (invalid_argument);
	void message(string msg, int offx, int offy, int windowId) throw (invalid_argument);
	void message(string msg, int offx, int offy, int windowId, Color)  throw (invalid_argument);

	void runGraph(unsigned long int  value, int id);
	
};

class OutCommand
{
public:
	OutCommand(int, int, string, OUT::Color);
	OutCommand(){};
	int x;
	int y;
	string msg;
	OUT::Color color;

};

class BoxInfo
{
public:
	OutPoint bori;
	OutPoint uple;

	int id;
	string title;

};


class BarGInfo
{
public:
	int id;
	OutPoint uple;
	OutPoint bori;
	int min;
	int max;
	string legend;
	string description;
};

class GraphInfo
{
public:
	OutPoint uple;
	OutPoint bori;

	int xMin;
	int xMax;
	int xdivisions;
	string xlegend;


	int yMin;
	int yMax;
	int ydivisions;
	string ylegend;
	
};

inline void OUT::move(int x, int y)
{
	cout << "\033[" << x << ';' << y << 'H';
}


#endif