#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

void exercicio1();
void exercicio2();
void exercicio3();


void exercicio1()
{
	GraphViewer *gv = new GraphViewer(600,600,true);
	gv->createWindow(600,600);
	gv->addNode(0);
	gv->addNode(1);
	gv->rearrange();

}

void exercicio2()
{

}

void exercicio3()
{

}
