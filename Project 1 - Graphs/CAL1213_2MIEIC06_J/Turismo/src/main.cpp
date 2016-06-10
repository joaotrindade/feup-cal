/*
 * main.cpp
 *
 *  Created on: 22 de Abr de 2013
 *      Author: ei11118
 */

#include <iostream>
#include "Graph.h"
#include "graphviewer.h"

#include <stdlib.h>

using namespace std;
GraphViewer *gv = new GraphViewer(600,600,true);
Graph<int> City_Graph;

/** Command prompt para o utilizador adicionar uma rua */
void promptAdicionaRua()
{
	//TODO: VERIFICAR SE O UTILIADOR NAO INSERE -1 e INSERE UM ID NAO EXISTENTE
	//TODO: VERIFICAR INPUTS
	int id_vertice1, id_vertice2, distancia;
	string nomeRua;
	cout << "Insira o vertice inicial caso já exista. Caso nao exista insira -1" << endl;
	cin>> id_vertice1;
	cout << "Insira o vertice de destino caso já exista. Caso nao exista insira -1" << endl;
	cin>> id_vertice2;
	cout << "Insira a distancia entre os vertices" << endl;
	cin>> distancia;
	cout << "Insira agora o nome da rua" << endl;
	cin.ignore();
	getline(cin, nomeRua);
	cout << "Nome: " << nomeRua << endl;

	if (id_vertice1 == -1)
	{
		City_Graph.incNextIndex();
		id_vertice1 = City_Graph.getNextIndex() ;
		City_Graph.addVertex(id_vertice1);
	}

	if (id_vertice2 == -1)
	{
		City_Graph.incNextIndex();
		id_vertice2 = City_Graph.getNextIndex() ;
		City_Graph.addVertex(id_vertice2);
	}
	cout << "Last Edge: " << last_edge_id << endl;
	City_Graph.addRua(id_vertice1, id_vertice2,distancia,0,nomeRua);
	City_Graph.addRua(id_vertice2, id_vertice1,distancia,0,nomeRua);
	gv->addEdge(City_Graph.getRuaId(nomeRua),id_vertice1,id_vertice2,EdgeType::UNDIRECTED);
	cout << "Saiu adiciona Rua" << endl;
}

/** Command prompt para o utilizador adicionar um monumento */
void promptAdicionaMonumento()
{

	string streetName, monumentName;
	cout << "Insira o nome da Rua em que se encontra o monumento: " << endl;
	cin.ignore();
	getline(cin, streetName);
	cout << "Insira o nome do momumento: " << endl;
	getline(cin, monumentName);
	cout << monumentName << endl;
	Monumento m1(monumentName);
	City_Graph.addMonumento(streetName, m1);
}
/** Command prompt para o utilizador remover uma rua */
void promptRemoverRua()
{
	string nomeRua;
	cout << "Insira o nome da rua a remover: " << endl;
	cin.ignore();
	getline(cin, nomeRua);
	gv->removeEdge(City_Graph.getRuaId(nomeRua));
	City_Graph.removeRua(nomeRua);

}
/** Command prompt para o utilizador remover um monumento */
void promptRemoverMonumento()
{
	string nomeMonumento;
	cout << "Insira o nome do monumento a remover: " << endl;
	cin.ignore();
	getline(cin, nomeMonumento);
	City_Graph.removeMonumento(nomeMonumento);
}

int main()
{
	string wait;


	gv->createWindow(600, 600);
	gv->rearrange();

	/*City_Graph.addVertex(0);
	gv->addNode(0);
	City_Graph.addVertex(1);
	gv->addNode(1);
	City_Graph.addVertex(2);
	gv->addNode(2);
	City_Graph.addVertex(3);
	gv->addNode(3);
	gv->rearrange();


	City_Graph.addRua(1,2,1,0,"Rua Trindade");
	gv->addEdge(1,1,2,EdgeType::UNDIRECTED);
	City_Graph.addRua(2,3,1,0,"Rua Principal");
	gv->addEdge(2,2,3,EdgeType::UNDIRECTED);
	City_Graph.addRua(3,1,1,0,"Rua dos Aliados");
	gv->addEdge(3,3,1,EdgeType::UNDIRECTED);

	gv->rearrange();
	Monumento Clerigos("Torre dos Clérigos"), PonteDLuis("Ponte D. Luis");

	City_Graph.addMonumento("Rua Trindade", Clerigos);
	City_Graph.addMonumento("Rua Trindade", PonteDLuis);
	City_Graph.addMonumento("Rua dos Aliados", Clerigos);*/


	int menu_option = 1;
	while (menu_option != 0)
	{
		City_Graph.adicionaLabels(gv);
		gv->rearrange();
		cout << "Bem vindo a aplicacao" << endl;
		cout << "1 - Adicionar Rua" << endl;
		cout << "2 - Remover Rua" << endl;
		cout << "3 - Listar Rua" << endl;
		cout << "4 - Adicionar Monumento" << endl;
		cout << "5 - Remover Monumento" << endl;
		cout << "6 - Listar Monumento" << endl << endl;
		cout << "7 - Gerar Caminho" << endl << endl;
		cout << "8 - Guardar em ficheiro" << endl;
		cout << "9 - Load de ficheiro" << endl << endl;
		cout << "0 - Encerrar" << endl;
		cout << endl << "Escolha opcao" << endl;

		cin >> menu_option;

		switch(menu_option)
		{
			case 1: promptAdicionaRua();
					break;
			case 2: promptRemoverRua();
					break;
			case 3: City_Graph.listarRuas();
					break;
			case 4: promptAdicionaMonumento();
					break;
			case 5: promptRemoverMonumento();
					break;
			case 6: City_Graph.listarMonumentos();
					break;
			case 7: City_Graph.gerarCaminho();
					break;
			case 8: City_Graph.saveMapa();
					break;
			case 9: City_Graph.loadMapa(gv);
					break;
			case 12: cout << City_Graph.getMonEdgesNumber() << endl;
					break;
			case 0: break;
			default: cout << "Entrada inválida!" << endl;
					break;
		}
		cin.ignore();
		getline(cin, wait);
	}


	return 0;
}
