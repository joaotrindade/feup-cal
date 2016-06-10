/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>
#include "Monumento.h"
#include "graphviewer.h"
using namespace std;

template <class T> class Edge;
template <class T> class Graph;

const int NOT_VISITED = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = 9999999;

int last_edge_id = 0;
int nextIndex = 0;
int totalMon = 0;

//template<class T> vector<Edge<T> > edgesMonumentos;

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	T info;
	vector<Edge<T>  > adj;
	bool visited;
	bool processing;
	int indegree;
	double dist;
	int set;
	Vertex* path;
	int num;
	int low;
public:
	Vertex(T in);
	Vertex(const Vertex<T> &v);
	friend class Graph<T>;

	void addEdge(Vertex<T> *dest, double w);
	void addEdge(Vertex<T> *dest, double w, double f);
	void addEdge(Vertex<T> *dest, double w, double f, string Nome_Rua);
	bool removeEdgeTo(Vertex<T> *d);

	T getInfo() const;
	void setInfo(T info);

	int getDist() const;
	int getIndegree() const;
	vector<Edge<T> > getAdj() const;
	Vertex<T>* getPath() const;

	int getNum() const;

	bool operator<(const Vertex<T> vertex);
	void updateEdgeFlow(unsigned int index, float f);
};


template <class T>
struct vertex_greater_than {
    bool operator()(Vertex<T> * a, Vertex<T> * b) const {
        return a->getDist() > b->getDist();
    }
};


template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	d->indegree--; //adicionado do exercicio 5
	typename vector<Edge<T> >::iterator it= adj.begin();
	typename vector<Edge<T> >::iterator ite= adj.end();
	while (it!=ite) {
		if (it->dest == d) {
			adj.erase(it);
			return true;
		}
		else it++;
	}
	return false;
}

//atualizado pelo exercício 5
template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false), processing(false), indegree(0), dist(0), num(0), low(0) {
	path = NULL;
}

template <class T>
Vertex<T>::Vertex(const Vertex<T> & in): info(in.info), visited(in.visited), processing(in.processing),
				indegree(in.indegree), dist(in.dist),num(in.num), low(in.low), path(NULL){}


template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w) {

	Edge<T> edgeD(dest,w);
	edgeD.orig = this;
	edgeD.id = last_edge_id;
	adj.push_back(edgeD);
	last_edge_id++;
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w, double f)
{

	Edge<T> edgeD(dest, w, f);
	edgeD.orig = this;
	edgeD.id = last_edge_id;
	adj.push_back(edgeD);
	last_edge_id++;
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dest, double w, double f, string Nome_Rua)
{
	cout << "Entrou | last_edge_id: " << last_edge_id << endl;
	Edge<T> edgeD(dest, w, f, Nome_Rua);
	edgeD.orig = this;
	edgeD.id = last_edge_id;
	adj.push_back(edgeD);
	last_edge_id++;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
int Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
vector<Edge<T> > Vertex<T>::getAdj() const {
	return this->adj;
}

template <class T>
Vertex<T>* Vertex<T>::getPath() const {
	return this->path;
}


template <class T>
void Vertex<T>::setInfo(T info) {
	this->info = info;
}

template <class T>
int Vertex<T>::getIndegree() const {
	return this->indegree;
}

template <class T>
int Vertex<T>::getNum() const {
	return this->num;
}

template <class T>
void Vertex<T>::updateEdgeFlow(unsigned int index, float f)
{
	if (index >= adj.size())
		return;
	adj[index].flow = f;
}



/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
	int id;
	Vertex<T> * dest;
	Vertex<T> * orig;
	vector<Monumento> monumentos;
	double weight;
	double flow;
	string nome_rua;
	bool visited;
public:
	Edge(Vertex<T> *d, double w, double f=0);
	Edge(Vertex<T> *d, double w, double f, string NomeRua);
	double getFlow() const;
	double getWeight() const;
	Vertex<T> *getDest() const;
	bool operator<(const Edge<T> &other) const;

	friend class Graph<T>;
	friend class Vertex<T>;
};


template <class T>
Edge<T>::Edge(Vertex<T> *d, double w, double f): dest(d), weight(w), flow(f){}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w, double f, string NomeRua): dest(d), weight(w), flow(f), nome_rua(NomeRua){}

template <class T>
double Edge<T>::getFlow() const {
	return flow;
}

template <class T>
double Edge<T>::getWeight() const {
	return weight;
}

template <class T>
Vertex<T>* Edge<T>::getDest() const {
	return dest;
}

template <class T>
bool Edge<T>::operator<(const Edge<T> &other) const {
	return this->weight < other.weight;
}

template <class T>
struct edge_greater_than {
    bool operator()(Edge<T> a, Edge<T>  b) const {
        return a.getWeight() > b.getWeight();
    }
};



/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	vector<Edge<T> *> edgeSet;
	void dfs(Vertex<T> *v, vector<T> &res) const;

	//exercicio 5
	int numCycles;
	void dfsVisit(Vertex<T> *v);
	void dfsVisit();
	void getPathTo(Vertex<T> *origin, list<T> &res);



	//exercicio 6


	int ** W;   //weight
	int ** P;   //path


public:
	//ADICIONADAS
	vector<Edge<T> > edgesMonumentos;
	void resetVisitedEdges();
	void listarRuas();
	void listarMonumentos();
	bool addRua(const T &sourc, const T &dest, double w,double f, string NomeRua);
	bool addMonumento(string NomeRua, Monumento monu1);
	void adicionaLabels(GraphViewer *graph);
	int getLastEdgeId(){return last_edge_id;}
	int getRuaId(string NomeRua);
	int getMonumentoRuaId(string NomeMonumento);
	void saveMapa();
	void loadMapa(GraphViewer *graph);
	void incNextIndex(){nextIndex++;}
	int getTotalMon();
	void chamaCircuito();
	int circuito(Vertex<T>* no_origem, vector<T>* vertices_percorridos, int pesoPercorrido, int mPercorridos);
	void loadEdgesVec();
	int getMonEdgesNumber();
	void gerarCaminho();
	void removeRua(string nomeRua);
	void removeMonumento(string nomeMonumento);
	//void changeNomeRua(string oldNome, string novoNome);
	//void changeNomeMonumento(string oldNome, string novoNome);

	//AULAS
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w,double f=0);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;
	int maxNewChildren(Vertex<T> *v, T &inf) const;
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;
	int getNextIndex(){return nextIndex;};

	//exercicio 5
	Vertex<T>* getVertex(const T &v) const;
	void resetIndegrees();
	vector<Vertex<T>*> getSources() const;
	int getNumCycles();
	vector<T> topologicalOrder();
	vector<T> getPath(const T &origin, const T &dest);
	void unweightedShortestPath(const T &v);
	bool isDAG();

	//exercicio 6
	void bellmanFordShortestPath(const T &s);
	void dijkstraShortestPath(const T &s);
	void floydWarshallShortestPath();
	int edgeCost(int vOrigIndex, int vDestIndex);
	vector<T> getfloydWarshallPath(const T &origin, const T &dest);
	void getfloydWarshallPathAux(int index1, int index2, vector<T> & res);

	//exercicio 8
	Graph<T> clone();
	void resetEdgeFlow();
	vector<Vertex<T>*> calculatePrim();
	vector<Vertex<T>*> calculateKruskal();
	vector<Vertex<T>*> calculateFordFulkerson(T source);
	float calculateFordFulkerson(Vertex<T>* current, Vertex<T>* parent, float min, Graph<T>* gf, Graph<T>* gr, vector<Vertex<T>*> visited);

};

// ADICIONADAS

/**
 * Funcao para remover um monumento dado o seu nome
 * @param nomeMonumento Nome do monumento a remover da cidade
 */
template<class T>
void Graph<T>::removeMonumento(string nomeMonumento)
{
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				vertexSet[i]->adj[j].visited = true;
				cout << vertexSet[i]->adj[j].nome_rua << endl;
				for(unsigned int k = 0; k < vertexSet[i]->adj[j].monumentos.size(); k++ )
				{
					if (vertexSet[i]->adj[j].monumentos.at(k).getNome() == nomeMonumento)
						vertexSet[i]->adj[j].monumentos.erase(vertexSet[i]->adj[j].monumentos.begin() + k);
				}
			}
		}
	}
}

/**
 * Funcao para remover uma rua, dado o seu nome
 * @param nomeRua - Nome da rua a remover
 */
template<class T>
void Graph<T>::removeRua(string nomeRua)
{
	int idSource, idDest;
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				if (vertexSet[i]->adj[j].nome_rua == nomeRua)
				{
					idSource = vertexSet[i]->adj[j].orig->info;
					idDest = vertexSet[i]->adj[j].dest->info;
				}
			}
		}
	}



	removeEdge(idSource, idDest);
	removeEdge(idDest, idSource);
}

/**
 * Funcao de geração de caminho ideal, utilizando algoritmo de Dijkstra
 */
template<class T>
void Graph<T>::gerarCaminho()
{
	loadEdgesVec();
	vector<int> caminho, backtrack;
	string nomeRua;
	int atual, temp, vertexDest, vertexDestNext, pesoEdge;
	int minDist = 99999, minVertex;
	atual = 1;
	while(edgesMonumentos.size() != 0)
	{
		minDist = 99999 ;
		dijkstraShortestPath(atual);
		for(unsigned int i = 0; i < edgesMonumentos.size(); i++ )
		{
			//cout << "i: " << i << ") Está a comparar " << edgesMonumentos.at(i).orig->info << " com " << edgesMonumentos.at(i).dest->info << endl;
			//cout << "Com os tamanhos " << getVertex(edgesMonumentos.at(i).orig->info)->dist << " contra " << getVertex(edgesMonumentos.at(i).dest->info)->dist << endl;

			int id, idNext, min, dist_A, idA, dist_B, idB;
			dist_A = getVertex(edgesMonumentos.at(i).orig->info)->dist ;
			idA = edgesMonumentos.at(i).orig->info ;
			dist_B = getVertex(edgesMonumentos.at(i).dest->info)->dist ;
			idB = edgesMonumentos.at(i).dest->info ;

			if (dist_A <= dist_B)
			{
				min = dist_A ;
				id = idA ;
				idNext = idB ;
			}
			else
			{
				min = dist_B ;
				id = idB ;
				idNext = idA ;
			}

			if (min < minDist)
			{
				nomeRua = edgesMonumentos.at(i).nome_rua;
				vertexDest = id;
				vertexDestNext = idNext;
				minDist = min;
			}
		}
		//cout << "VertexDest: " << vertexDest << endl;

		caminho.push_back(atual);
		temp = getVertex(vertexDest)->info;

		for (int k = 0 ; k < backtrack.size() ; k++)
			backtrack.at(k) = -1;

		while(temp != atual)
		{
			backtrack.push_back(temp);
			temp = (getVertex(temp)->path->info);
		}

		for (int k = backtrack.size() - 1 ; k >= 0 ; k--)
			caminho.push_back(backtrack.at(k));

		caminho.push_back(vertexDestNext);
		atual = vertexDestNext;

		for(unsigned int j = 0; j < edgesMonumentos.size(); j++ )
		{
			if (edgesMonumentos.at(j).nome_rua == nomeRua)
			{
				edgesMonumentos.erase(edgesMonumentos.begin()+j);
				break;
			}
		}
		for(unsigned int j = 0; j < edgesMonumentos.size(); j++ )
		{
			if (edgesMonumentos.at(j).nome_rua == nomeRua)
			{
				edgesMonumentos.erase(edgesMonumentos.begin()+j);
				break;
			}
		}
	}

	// dijkstra do final para o vértice 1;

	dijkstraShortestPath(caminho.at(caminho.size() - 1));

	for (int k = 0 ; k < backtrack.size() ; k++)
		backtrack.at(k) = -1;

	temp = 1 ;

	while(temp != caminho.at(caminho.size() - 1))
	{
		backtrack.push_back(temp);
		temp = (getVertex(temp)->path->info);
	}

	for (int k = backtrack.size() - 1 ; k >= 0 ; k--)
		caminho.push_back(backtrack.at(k));

	// dijkstra do final para o vértice 1;

	cout << "Caminho a percorrer:" << endl;

	cout << caminho.at(0) << " ";
	for(unsigned int k = 1; k < caminho.size(); k++)
		if ((caminho.at(k) != -1) && (caminho.at(k) != caminho.at(k - 1))) cout << caminho.at(k) << " ";
	cout << endl;

}

/*
template<class T>
void Graph<T>::chamaCircuito()
{
	totalMon = getTotalMon();
	resetVisitedEdges();
	vector<int> resultado, vazio;
	vazio.push_back(0);
	//resultado = circuito(0,vazio,0,0);
}*/

template<class T>
int Graph<T>::getMonEdgesNumber()
{
	return edgesMonumentos.size();
}

/**
 * Funcao para importar todos as ruas com monumentos a visitar para um vector de Edges
 */
template<class T>
void Graph<T>::loadEdgesVec()
{

	this->resetVisitedEdges();

	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				if (vertexSet[i]->adj[j].monumentos.size() != 0) edgesMonumentos.push_back(vertexSet[i]->adj[j]); //não está a adicionar;
				vertexSet[i]->adj[j].visited = true;
			}
		}
	}

}

/**
 * Numero total de monumentos a visitar
 */
template<class T>
int Graph<T>::getTotalMon()
{
	int resultado = 0;
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				resultado += vertexSet[i]->adj[j].monumentos.size();
			}
		}
	}
	return resultado;
}

/**
 * Listagem das ruas da cidade
 */
template <class T>
void Graph<T>::listarRuas()
{
	cout << endl << endl;
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				vertexSet[i]->adj[j].visited = true;
				cout << "Nome rua: " << vertexSet[i]->adj[j].nome_rua << ". Distancia: " << vertexSet[i]->adj[j].weight << endl;
			}
		}
	}
	cout << "Saiu Listar" << endl;
}

/**
 * Save do Mapa atual
 */
template<class T>
void Graph<T>::saveMapa()
{
	string filename = "mapa.txt";
	int found_nome = 0;
	ofstream outfile(filename.c_str());
	vector<string> nomes_existentes;
	//cout << "entrou save mapa" << endl;
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{

				found_nome = 0;
				nomes_existentes.push_back(vertexSet[i]->adj[j].nome_rua);
				for(unsigned int k = 0; k < nomes_existentes.size(); k++)
				{
					//cout << "Comparação: " << nomes_existentes.at(k) << " e " << nomeRua << endl;
					if (vertexSet[i]->adj[j].nome_rua == nomes_existentes.at(k))
					{
						//cout << "^ é repetido!" << endl;
						found_nome += 1;
					}
				}
				if (found_nome == 1)
				{
					outfile << vertexSet[i]->info << " " << vertexSet[i]->adj[j].dest->info << " " << vertexSet[i]->adj[j].weight << " " << vertexSet[i]->adj[j].nome_rua << "@"  ;
					vertexSet[i]->adj[j].visited = true;
					//cout << vertexSet[i]->adj[j].nome_rua << endl;
					for(unsigned int k = 0; k < vertexSet[i]->adj[j].monumentos.size(); k++ )
					{
						//cout << vertexSet[i]->adj[j].monumentos.at(k).getNome() << endl;
						outfile << vertexSet[i]->adj[j].monumentos.at(k).getNome() << "#";
					}
					outfile << endl;
				}
			}
		}
	}
}

/**
 * Load de um mapa em memória
 */
template <class T>
void Graph<T>::loadMapa(GraphViewer *graph)
{
	vector<string>nomes_existentes;
	//bool found_nome;
	int highest_vertex_id = -1;
	string filename = "mapa.txt";
	string merda;
	int x_pos,id_vertice1, id_vertice2, weight;
	string idVertice1, idVertice2, weightString, nomeRua, nomeMonumento;
	string dados_temp;
	char tempChar;
	ifstream infile(filename.c_str());
	getline(infile, dados_temp);
	while (!infile.eof())
	{

		idVertice1 = "";
		idVertice2 = "";
		weightString ="";
		nomeRua = "";
		nomeMonumento = "";

		x_pos = 0;



		//cout << "Saida: " << dados_temp << endl;
		tempChar = dados_temp[x_pos];
		while (tempChar != ' ')
		{
			idVertice1 += tempChar;
			x_pos++;
			tempChar = dados_temp[x_pos];
		}
		//cout << "Vertice1: " << idVertice1 << endl;
		id_vertice1 = atoi(idVertice1.c_str());
		if (id_vertice1 > highest_vertex_id) highest_vertex_id = id_vertice1;

		if (getVertex(id_vertice1) == NULL)
		{
			cout << "Vai adicionar Vertice - indice : " <<  id_vertice1 << endl;
			addVertex(id_vertice1);
			//graph->addNode(id_vertice1);
		}
		cout << "Vertice1: " << id_vertice1 << endl;

		x_pos++;
		tempChar = dados_temp[x_pos];

		while (tempChar != ' ')
		{
			idVertice2 += tempChar;
			x_pos++;
			tempChar = dados_temp[x_pos];

		}
		cout << "Vertice2: " << idVertice2 << endl;
		id_vertice2 = atoi(idVertice2.c_str());
		if (id_vertice2 > highest_vertex_id) highest_vertex_id = id_vertice2;
		if (getVertex(id_vertice2) == NULL)
		{
			cout << "Vai adicionar Vertice - indice : " <<  id_vertice2 << endl;
			addVertex(id_vertice2);
			//graph->addNode(id_vertice2);
		}
		//cout << "Vertice2: " << id_vertice2 << endl;


		x_pos++;
		tempChar = dados_temp[x_pos];

		while (tempChar != ' ')
		{
			weightString += tempChar;
			x_pos++;
			tempChar = dados_temp[x_pos];
		}
		//cout << "weightString: " << weightString << endl;
		weight = atoi(weightString.c_str());

		x_pos++;
		tempChar = dados_temp[x_pos];

		while (tempChar != '@')
		{
			nomeRua += tempChar;
			x_pos++;
			tempChar = dados_temp[x_pos];
		}
		//cout << "nomeRua: " << nomeRua << endl;
		bool adicionou = false;
		adicionou = addRua(id_vertice1, id_vertice2, weight,0,nomeRua);
		addRua(id_vertice2, id_vertice1, weight,0,nomeRua);
		nomes_existentes.push_back(nomeRua);
		cout << "Adicionou" << adicionou << endl;
		//getLastEdgeId();
		//cout << "nadaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << endl;
		//cout << "LASTEDGE: " << getLastEdgeId() << " | ID_VERTICE 1: " << id_vertice1 <<  " | ID_VERTICE 2: " << id_vertice2 << endl;
		//cout << endl;
		int found_nome = 0 ;
		//cout << nomes_existentes.size() << endl;
		for(unsigned int k = 0; k < nomes_existentes.size(); k++)
		{
			//cout << "Comparação: " << nomes_existentes.at(k) << " e " << nomeRua << endl;
			if (nomeRua == nomes_existentes.at(k))
			{
				//cout << "^ é repetido!" << endl;
				found_nome += 1;
			}
		}
		if (found_nome == 1)
		{
			//cout << nomeRua << "não é repetido!" << endl;
			graph->addEdge(getRuaId(nomeRua),id_vertice1,id_vertice2,EdgeType::UNDIRECTED);
		}
		//TODO: ERRO PARVO

		//graph->rearrange();

		x_pos++;
		tempChar = dados_temp[x_pos];
		while(tempChar != '\0')
		{
			//cout << "entrou" << endl;
			if(tempChar == '#')
			{
				//cout << "nome monumento: " << nomeMonumento << endl;

				Monumento temp(nomeMonumento);
				addMonumento(nomeRua, temp);
				nomeMonumento = "";
				x_pos++;
				tempChar = dados_temp[x_pos];
			}
			else
			{
				nomeMonumento += tempChar;
				x_pos++;
				tempChar = dados_temp[x_pos];
			}

		}
		getline(infile, dados_temp);

	}

	nextIndex = highest_vertex_id;
}

/**
 * Listagem de monumentos da cidade
 */
template <class T>
void Graph<T>::listarMonumentos()
{
	cout << endl << endl;
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				vertexSet[i]->adj[j].visited = true;
				cout << vertexSet[i]->adj[j].nome_rua << endl;
				for(unsigned int k = 0; k < vertexSet[i]->adj[j].monumentos.size(); k++ )
				{
					cout << vertexSet[i]->adj[j].monumentos.at(k).getNome() << endl;
				}
			}
			cout << endl << endl;
		}
	}
	cout << "Saiu Listar Monumentos" << endl;
}


/**
 * Adiciona uma rua a cidade, dados os vertices de origem, destino, distancia da rua, valor zero, e nome da rua
 */
template <class T>
bool Graph<T>::addRua(const T &sourc, const T &dest, double w,double f, string NomeRua)
{
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
		typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
		int found=0;
		Vertex<T> *vS, *vD;
		while (found!=2 && it!=ite ) {
			if ( (*it)->info == sourc )
				{ vS=*it; found++;}
			if ( (*it)->info == dest )
				{ vD=*it; found++;}
			it ++;
		}
		if (found!=2) return false;
		vD->indegree++;
		vS->addEdge(vD,w,f, NomeRua);

		return true;
}

/**
 * Devolve o ID de uma rua, dado o seu nome
 * @param NomeRua nome da rua pretendido
 * @return ID da rua pretendida
 */
template<class T>
int  Graph<T>::getRuaId(string NomeRua)
{
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				if (vertexSet[i]->adj[j].nome_rua == NomeRua)
				{
					return vertexSet[i]->adj[j].id;
				}

			}
		}
	}
	return -1;
}

/**
 * Devolve o ID de uma rua em que existe um monumento com determinado nome
 * @param NomeMonumento Nome do monumento
 * @return ID da rua com o respectivo monumento
 */
template<class T>
int Graph<T>::getMonumentoRuaId(string NomeMonumento)
{
	this->resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "Entrou" << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if (vertexSet[i]->adj[j].visited == false)
			{
				vertexSet[i]->adj[j].visited = true;
				cout << vertexSet[i]->adj[j].nome_rua << endl;
				for(unsigned int k = 0; k < vertexSet[i]->adj[j].monumentos.size(); k++ )
				{
					if (vertexSet[i]->adj[j].monumentos.at(k).getNome() == NomeMonumento)
					{
						return vertexSet[i]->adj[j].id;
					}
				}
			}
		}
	}
	return -1;
}

/**
 * Adiciona um dado monumento, numa rua
 * @param NomeRua Nome da rua onde adicionar o monumento
 * @param monu1 Monumento a adicionar
 */
template <class T>
bool Graph<T>::addMonumento(string NomeRua, Monumento monu1)
{
	bool found = false;
	cout << "ENTROU ADD MONUMENTO " << endl;
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			if ((vertexSet[i]->adj[j].nome_rua) == NomeRua)
			{

				vertexSet[i]->adj[j].monumentos.push_back(monu1);
				found = true;
			}
		}
	}
	return found;
}

/**
 * Coloca todas as ruas como não-visitadas
 */
template<class T>
void Graph<T>::resetVisitedEdges()
{
	for(unsigned int i = 0; i<vertexSet.size(); i++)
	{
		//cout << "entrou i:" << i << " / " << vertexSet.size() << endl;
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
		{
			//cout << "entrou interior  j:" << j << " / " << vertexSet[i]->adj.size() << endl;
			vertexSet[i]->adj[j].visited = false;
		}
		//cout << "Saiu1" << endl;
	}
	//cout << "Saiu2" << endl;
}

/**
 * Adiciona todas as labels ao Graph-VIEWER
 * @param graph Graph-Viewer onde atualizar
 */
template<class T>
void Graph<T>::adicionaLabels(GraphViewer *graph)
{
	string resultado;
	char toto[100];
	resetVisitedEdges();
	for(unsigned int i = 0; i<vertexSet.size(); i++)
		{
			for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++)
			{
				if (vertexSet[i]->adj[j].visited == false)
				{
					resultado = "";
					itoa (vertexSet[i]->adj[j].weight, toto, 10) ;
					resultado = toto;
					resultado += "->";
					resultado += vertexSet[i]->adj[j].nome_rua ;
					resultado += " |" ;

					for(unsigned int k = 0; k < vertexSet[i]->adj[j].monumentos.size(); k++)
					{
						resultado += vertexSet[i]->adj[j].monumentos.at(k).getNome();
						resultado += " |" ;
					}

					graph->setEdgeLabel(vertexSet[i]->adj[j].id, resultado );
				}
			}
		}

}

// AULAS


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}
template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
int Graph<T>::getNumCycles() {
	numCycles = 0;
	dfsVisit();
	return this->numCycles;
}

template <class T>
bool Graph<T>::isDAG() {
	return (getNumCycles() == 0);
}

template <class T>
bool Graph<T>::addVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++)
		if ((*it)->info == in) return false;
	Vertex<T> *v1 = new Vertex<T>(in);
	vertexSet.push_back(v1);
	return true;
}

template <class T>
bool Graph<T>::removeVertex(const T &in) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	for (; it!=ite; it++) {
		if ((*it)->info == in) {
			Vertex<T> * v= *it;
			vertexSet.erase(it);
			typename vector<Vertex<T>*>::iterator it1= vertexSet.begin();
			typename vector<Vertex<T>*>::iterator it1e= vertexSet.end();
			for (; it1!=it1e; it1++) {
				(*it1)->removeEdgeTo(v);
			}

			typename vector<Edge<T> >::iterator itAdj= v->adj.begin();
			typename vector<Edge<T> >::iterator itAdje= v->adj.end();
			for (; itAdj!=itAdje; itAdj++) {
				itAdj->dest->indegree--;
			}
			delete v;
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w, double f) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
			{ vS=*it; found++;}
		if ( (*it)->info == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2) return false;
	vD->indegree++;
	vS->addEdge(vD,w,f);

	return true;
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	typename vector<Vertex<T>*>::iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::iterator ite= vertexSet.end();
	int found=0;
	Vertex<T> *vS, *vD;
	while (found!=2 && it!=ite ) {
		if ( (*it)->info == sourc )
			{ vS=*it; found++;}
		if ( (*it)->info == dest )
			{ vD=*it; found++;}
		it ++;
	}
	if (found!=2)
		return false;

	vD->indegree--;

	return vS->removeEdgeTo(vD);
}

template <class T>
vector<T> Graph<T>::dfs() const {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	vector<T> res;
	it=vertexSet.begin();
	for (; it !=ite; it++)
	    if ( (*it)->visited==false )
	    	dfs(*it,res);
	return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v,vector<T> &res) const {
	v->visited = true;
	res.push_back(v->info);
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++)
	    if ( it->dest->visited == false ){
	    	//cout << "ok ";
	    	dfs(it->dest, res);
	    }
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	q.push(v);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
			}
		}
	}
	return res;
}

template <class T>
int Graph<T>::maxNewChildren(Vertex<T> *v, T &inf) const {
	vector<T> res;
	queue<Vertex<T> *> q;
	queue<int> level;
	int maxChildren=0;
	inf =v->info;
	q.push(v);
	level.push(0);
	v->visited = true;
	while (!q.empty()) {
		Vertex<T> *v1 = q.front();
		q.pop();
		res.push_back(v1->info);
		int l=level.front();
		level.pop(); l++;
		int nChildren=0;
		typename vector<Edge<T> >::iterator it=v1->adj.begin();
		typename vector<Edge<T> >::iterator ite=v1->adj.end();
		for (; it!=ite; it++) {
			Vertex<T> *d = it->dest;
			if (d->visited==false) {
				d->visited=true;
				q.push(d);
				level.push(l);
				nChildren++;
			}
		}
		if (nChildren>maxChildren) {
			maxChildren=nChildren;
			inf = v1->info;
		}
	}
	return maxChildren;
}

template <class T>
Vertex<T>* Graph<T>::getVertex(const T &v) const {
	for(unsigned int i = 0; i < vertexSet.size(); i++)
		if (vertexSet[i]->info == v) return vertexSet[i];
	return NULL;
}

template<class T>
void Graph<T>::resetIndegrees() {
	//colocar todos os indegree em 0;
	for(unsigned int i = 0; i < vertexSet.size(); i++) vertexSet[i]->indegree = 0;

	//actualizar os indegree
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		//percorrer o vector de Edges, e actualizar indegree
		for(unsigned int j = 0; j < vertexSet[i]->adj.size(); j++) {
			vertexSet[i]->adj[j].dest->indegree++;
		}
	}
}

template<class T>
vector<Vertex<T>*> Graph<T>::getSources() const {
	vector< Vertex<T>* > buffer;
	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		if( vertexSet[i]->indegree == 0 ) buffer.push_back( vertexSet[i] );
	}
	return buffer;
}

template <class T>
void Graph<T>::dfsVisit() {
	typename vector<Vertex<T>*>::const_iterator it= vertexSet.begin();
	typename vector<Vertex<T>*>::const_iterator ite= vertexSet.end();
	for (; it !=ite; it++)
		(*it)->visited=false;
	it=vertexSet.begin();
	for (; it !=ite; it++)
	    if ( (*it)->visited==false )
	    	dfsVisit(*it);
}

template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v) {
	v->processing = true;
	v->visited = true;
	typename vector<Edge<T> >::iterator it= (v->adj).begin();
	typename vector<Edge<T> >::iterator ite= (v->adj).end();
	for (; it !=ite; it++) {
		if ( it->dest->processing == true) numCycles++;
	    if ( it->dest->visited == false ){
	    	dfsVisit(it->dest);
	    }
	}
	v->processing = false;
}

template<class T>
vector<T> Graph<T>::topologicalOrder() {
	//vetor com o resultado da ordenacao
	vector<T> res;

	//verificar se é um DAG
	if( getNumCycles() > 0 ) {
		cout << "Ordenacao Impossivel!" << endl;
		return res;
	}

	//garantir que os "indegree" estao inicializados corretamente
	this->resetIndegrees();

	queue<Vertex<T>*> q;

	vector<Vertex<T>*> sources = getSources();
	while( !sources.empty() ) {
		q.push( sources.back() );
		sources.pop_back();
	}


	//processar fontes
	while( !q.empty() ) {
		Vertex<T>* v = q.front();
		q.pop();

		res.push_back(v->info);

		for(unsigned int i = 0; i < v->adj.size(); i++) {
			v->adj[i].dest->indegree--;
			if( v->adj[i].dest->indegree == 0) q.push( v->adj[i].dest );
		}
	}

	//testar se o procedimento foi bem sucedido
	if ( res.size() != vertexSet.size() ) {
		while( !res.empty() ) res.pop_back();
	}

	//garantir que os "indegree" ficam atualizados ao final
	this->resetIndegrees();

	return res;
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest){

	list<T> buffer;
	Vertex<T>* v = getVertex(dest);

	buffer.push_front(v->info);
	while ( v->path != NULL &&  v->path->info != origin) {
		v = v->path;
		buffer.push_front(v->info);
	}
	if( v->path != NULL )
		buffer.push_front(v->path->info);


	vector<T> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
	return res;
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &origin, const T &dest){

	int originIndex = -1, destinationIndex = -1;

	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		if(vertexSet[i]->info == origin)
			originIndex = i;
		if(vertexSet[i]->info == dest)
			destinationIndex = i;

		if(originIndex != -1 && destinationIndex != -1)
			break;
	}


	vector<T> res;

	//se nao foi encontrada solucao possivel, retorna lista vazia
	if(W[originIndex][destinationIndex] == INT_INFINITY)
		return res;

	res.push_back(vertexSet[originIndex]->info);

	//se houver pontos intermedios...
	if(P[originIndex][destinationIndex] != -1)
	{
		int intermedIndex = P[originIndex][destinationIndex];

		getfloydWarshallPathAux(originIndex, intermedIndex, res);

		res.push_back(vertexSet[intermedIndex]->info);

		getfloydWarshallPathAux(intermedIndex,destinationIndex, res);
	}

	res.push_back(vertexSet[destinationIndex]->info);


	return res;
}

template<class T>
void Graph<T>::getfloydWarshallPathAux(int index1, int index2, vector<T> & res)
{
	if(P[index1][index2] != -1)
	{
		getfloydWarshallPathAux(index1, P[index1][index2], res);

		res.push_back(vertexSet[P[index1][index2]]->info);

		getfloydWarshallPathAux(P[index1][index2],index2, res);
	}
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex<T>* w = v->adj[i].dest;
			if( w->dist == INT_INFINITY ) {
				w->dist = v->dist + 1;
				w->path = v;
				q.push(w);
			}
		}
	}
}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex<T>* w = v->adj[i].dest;
			if(v->dist + v->adj[i].weight < w->dist) {
				w->dist = v->dist + v->adj[i].weight;
				w->path = v;
				q.push(w);
			}
		}
	}
}

template<class T>
void Graph<T>::dijkstraShortestPath(const T &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processing = false;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;

	vector< Vertex<T>* > pq;
	pq.push_back(v);

	make_heap(pq.begin(), pq.end());


	while( !pq.empty() ) {

		v = pq.front();
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();

		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex<T>* w = v->adj[i].dest;

			if(v->dist + v->adj[i].weight < w->dist ) {

				w->dist = v->dist + v->adj[i].weight;
				w->path = v;

				//se já estiver na lista, apenas a actualiza
				if(!w->processing)
				{
					w->processing = true;
					pq.push_back(w);
				}

				make_heap (pq.begin(),pq.end(),vertex_greater_than<T>());
			}
		}
	}
}

template<class T>
int Graph<T>::edgeCost(int vOrigIndex, int vDestIndex)
{
	if(vertexSet[vOrigIndex] == vertexSet[vDestIndex])
		return 0;

	for(unsigned int i = 0; i < vertexSet[vOrigIndex]->adj.size(); i++)
	{
		if(vertexSet[vOrigIndex]->adj[i].dest == vertexSet[vDestIndex])
			return vertexSet[vOrigIndex]->adj[i].weight;
	}

	return INT_INFINITY;
}

void printSquareArray(int ** arr, unsigned int size)
{
	for(unsigned int k = 0; k < size; k++)
	{
		if(k == 0)
		{
			cout <<  "   ";
			for(unsigned int i = 0; i < size; i++)
				cout <<  " " << i+1 << " ";
			cout << endl;
		}

		for(unsigned int i = 0; i < size; i++)
		{
			if(i == 0)
				cout <<  " " << k+1 << " ";

			if(arr[k][i] == INT_INFINITY)
				cout << " - ";
			else
				cout <<  " " << arr[k][i] << " ";
		}

		cout << endl;
	}
}

template<class T>
void Graph<T>::floydWarshallShortestPath() {

	W = new int * [vertexSet.size()];
	P = new int * [vertexSet.size()];
	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		W[i] = new int[vertexSet.size()];
		P[i] = new int[vertexSet.size()];
		for(unsigned int j = 0; j < vertexSet.size(); j++)
		{
			W[i][j] = edgeCost(i,j);
			P[i][j] = -1;
		}
	}

	for(unsigned int k = 0; k < vertexSet.size(); k++)
		for(unsigned int i = 0; i < vertexSet.size(); i++)
			for(unsigned int j = 0; j < vertexSet.size(); j++)
			{
				//se somarmos qualquer coisa ao valor INT_INFINITY, ocorre overflow, o que resulta num valor negativo, logo nem convém considerar essa soma
				if(W[i][k] == INT_INFINITY || W[k][j] == INT_INFINITY)
					continue;

				int val = min ( W[i][j], W[i][k]+W[k][j] );
				if(val != W[i][j])
				{
					W[i][j] = val;
					P[i][j] = k;
				}
			}

}

template <class T>
void Graph<T>::resetEdgeFlow()
{
	for (unsigned int i = 0; i < vertexSet.size(); i++)
	{
		for (unsigned int a = 0; a < vertexSet[i]->adj.size(); a++)
			vertexSet[i]->adj[a].flow = 0;
	}
}

template <class T>
Graph<T> Graph<T>::clone()
{
	Graph<T> ret;
	for (unsigned int i = 0; i < this->vertexSet.size(); i++)
		ret.addVertex(this->vertexSet[i]->info);

	for (unsigned int i = 0; i < this->vertexSet.size(); i++)
	{
		vector<Edge<T> > edges = this->vertexSet[i]->adj;
		for (unsigned int a = 0; a < edges.size(); a++)
			ret.addEdge(this->vertexSet[i]->info, edges[a].dest->info, edges[a].weight, edges[a].flow);
	}

	return ret;
}

template <class T>
vector<Vertex<T>* > Graph<T>::calculatePrim()
{
	for(unsigned int i = 0; i < this->vertexSet.size(); i++) {
		this->vertexSet[i]->path = NULL;
		this->vertexSet[i]->dist = INT_INFINITY;
		this->vertexSet[i]->visited = false;
	}

	Vertex<T>* v = this->vertexSet[0];
	v->dist = 0;

	vector< Vertex<T>* > pq;
	pq.push_back(v);

	make_heap(pq.begin(), pq.end(), vertex_greater_than<int>());

	// a kind of Dijkstra modified
	while( !pq.empty() ) {

		v = pq.front();
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();
		if(!v->visited)
		{
			v->visited = true;
			for(unsigned int i = 0; i < v->adj.size(); i++) {

				Vertex<T>* w = v->adj[i].dest;

				if(!w->visited)
				{
					if(v->adj[i].weight < w->dist ) {
						w->dist = v->adj[i].weight;
						w->path = v;
						pq.push_back(w);
					}
				}
			}
		}
		make_heap(pq.begin(), pq.end(), vertex_greater_than<int>());
	}
	return this->vertexSet;
}

template <class T>
vector<Vertex<T>*> Graph<T>::calculateKruskal()
{
	unsigned edges_accepted = 0;
	if (vertexSet.size() == 0)
		return this->vertexSet;

	vector<Vertex<T>*> forest;
	for (unsigned int i = 0; i < this->vertexSet.size(); i++)
	{
		Vertex<T>* v = new Vertex<T>(this->vertexSet[i]->info);
		v->set = i;
		forest.push_back(v);
	}

	//Initialize the list of edges
	vector<Edge<T> > allEdges;
	for (unsigned int i = 0; i < this->vertexSet.size(); i++)
	{
		Vertex<T>* v = this->vertexSet[i];
		v->set = i;
		for (unsigned int a = 0; a < v->adj.size(); a++)
			allEdges.push_back(v->adj[a]);
	}

	//Make heap from vector
	make_heap(allEdges.begin(), allEdges.end(), edge_greater_than<T>());

	while (edges_accepted < vertexSet.size()-1)
	{
		sort_heap(allEdges.begin(), allEdges.end());

		Edge<T> minEdge = allEdges[0];		// get edge with minimum weight
		allEdges.erase(allEdges.begin());

		//Get the vertices
		T o = minEdge.orig->info;
		T d = minEdge.dest->info;

		Vertex<T>* origin = NULL;
		Vertex<T>* dest = NULL;

		for (unsigned int i = 0; i < forest.size(); i++)
		{
			if (o == forest[i]->info)
				origin = forest[i];
			if (d == forest[i]->info)
				dest = forest[i];
		}

		if (origin->set != dest->set)
		{
			int minSet = min(origin->set, dest->set);
			int maxSet = max(origin->set, dest->set);
			for (unsigned int k = 0; k < forest.size(); k++) {
				if (forest[k]->set == maxSet)
					forest[k]->set = minSet;
			}
			if (dest->path == NULL)
				dest->path=origin;
			else if (origin->path == NULL)
				origin->path=dest;
			edges_accepted++;
			cout << "Adding edge from vertex " << origin->info << " to vertex " << dest->info << endl;
		}
	}

	return forest;
}

template <class T>
vector<Vertex<T>*> Graph<T>::calculateFordFulkerson(T source)
{
	Graph<T> gr = this->clone();
	Graph<T> gf = this->clone();
	gf.resetEdgeFlow();

	Vertex<T>* start = gr.getVertex(source);
	if (start==NULL) throw -1;

	while(true)
	{
		vector<Vertex<T>*> visited;
		visited.push_back(start);

		float res = this->calculateFordFulkerson(start, NULL, 99999, &gf, &gr, visited);
		if (res < 0)
		{
			cout << "*** All paths were searched. Terminating!" << endl;
			break;
		}
	}

	return gf.vertexSet;
}

template <class T>
float Graph<T>::calculateFordFulkerson(Vertex<T>* current, Vertex<T>* parent, float min, Graph<T>* gf, Graph<T>* gr, vector<Vertex<T>*> visited)
{
	cout << "*** Currently in vertex " << current->info << endl;
	cout << "*** Minimum value is " << min << endl;

	vector<Edge<T> > edges = current->adj;

	for (unsigned int i = 0; i < edges.size(); i++)
	{
		Edge<T> e = edges[i];

		cout << "*** Searching edge to vertex " << e.dest->info << endl;

		Vertex<T>* dest = edges[i].dest;
		Vertex<T>* destInGraph = this->getVertex(edges[i].dest->info);

		bool alreadyVisited = false;
		for (unsigned int e = 0; e < visited.size(); e++)
		{
			if (dest->info == visited[e]->info)
			{
				cout << "*** Detected that " << dest->info << " was already visited. Will now continue with the next one!" << endl;
				alreadyVisited = true;
				break;
			}
		}

		if (alreadyVisited)
			continue;

		if (e.flow < min)
		{
			min = e.flow;
			cout << "*** Minimum flow updated to: " << min << endl;
		}

		visited.push_back(dest);

		if (destInGraph->adj.size() == 0)
		{
			cout << "*** Reached a destination: " << dest->info << endl;

			//Upon the return, update the edges with the min value
			float newVal = e.flow - min;
			if (newVal == 0)
			{
				cout << "*** Deleting edge from " << current->info << " to " << dest->info << " since the val is 0!" << endl;
				gr->removeEdge(current->info, dest->info);
			}
			else
				current->updateEdgeFlow(i, e.flow - min);


			//Check to see if this edge already exists
			bool edgeExists = false;
			unsigned int a = 0;
			for (a = 0; a < dest->adj.size(); a++)
			{
				if (dest->adj[a].dest->info == current->info)
				{
					edgeExists = true;
					break;
				}
			}

			if (!edgeExists)
			{
				gr->addEdge(dest->info, current->info, e.weight, min);
				cout << "*** Edge from " << dest->info << " to " << current->info << " does not exist in Gr... Adding with " << min << endl;
			}
			else
			{
				dest->updateEdgeFlow(a, dest->adj[a].flow + min);
				cout << "*** Edge from " << dest->info << " to " << current->info << " already exists in Gr... Updating to: " << dest->adj[a].flow + min << endl;
			}

			//Search for the correct edge on Gf since both graphs may be unsynchronized
			Vertex<T>* updatedVertex = gf->getVertex(current->info);
			for (a = 0; a < current->adj.size(); a++)
			{
				if (current->adj[a].dest->info == dest->info)
					break;
			}
			float prevValue = updatedVertex->adj[a].flow;

			updatedVertex->updateEdgeFlow(a, prevValue + min);
			Edge<T> updatedEdge = updatedVertex->adj[a];

			cout << "*** Setting a new edge flow value to the final graph from " << updatedVertex->info << " to " << updatedEdge.dest->info << " with a flow value of " << updatedEdge.flow << endl;

			cout << "****** Returning minimum value of : " << min << endl;
			return min;
		}
		else
		{
			cout << "*** Will enter recursion with new current: " << dest->info << endl;
			float newmin = this->calculateFordFulkerson(dest, current, min, gf, gr, visited);

			cout << "*** Returned from recursion where new current was: " << dest->info << endl;
			cout << "*** New minimum value is: " << newmin << endl;

			if (newmin < 0)
			{
				cout << "*** There are no more paths to the destination. Exiting with new minimum: " << newmin << endl;
				continue;
			}

			//Upon the return, update the edges with the min value
			float newVal = e.flow - newmin;
			if (newVal == 0)
			{
				cout << "*** Deleting edge from " << current->info << " to " << dest->info << " since the val is 0!" << endl;
				gr->removeEdge(current->info, dest->info);
			}
			else
				current->updateEdgeFlow(i, e.flow - newmin);

			//Check to see if this edge already exists
			bool edgeExists = false;
			unsigned int a = 0;
			for (a = 0; a < dest->adj.size(); a++)
			{
				if (dest->adj[a].dest->info == current->info)
				{
					edgeExists = true;
					break;
				}
			}

			if (!edgeExists)
			{
				gr->addEdge(dest->info, current->info, e.weight, newmin);
				cout << "*** Edge from " << dest->info << " to " << current->info << " does not exist in Gr... Adding with " << newmin << endl;
			}
			else
			{
				float newVal = dest->adj[a].flow + newmin;
				dest->updateEdgeFlow(a, newVal);
				cout << "*** Edge from " << dest->info << " to " << current->info << " already exists in Gr... Updating to: " << newVal << endl;
			}

			//Search for the correct edge on Gf since both graphs may be unsynchronized
			Vertex<T>* updatedVertex = gf->getVertex(current->info);
			for (a = 0; a < updatedVertex->adj.size(); a++)
			{
				if (updatedVertex->adj[a].dest->info == dest->info)
					break;
			}
			if (a < updatedVertex->adj.size())
			{
				float prevValue = updatedVertex->adj[a].flow;

				updatedVertex->updateEdgeFlow(a, prevValue + newmin);
				Edge<T> updatedEdge = updatedVertex->adj[a];

				cout << "*** Setting a new edge flow value to the final graph from " << updatedVertex->info << " to " << updatedEdge.dest->info << " with a flow value of " << updatedEdge.flow << endl;
			}
			else {
				cout << "*** Didn't find the edge on Gf. Probably this was a REVERSE EDGE created in Gr. Find corresponding in Gf..." << endl;
				Vertex<T>* reverseVertex = gf->getVertex(dest->info);
				for (a = 0; a < reverseVertex->adj.size(); a++)
				{
					if (reverseVertex->adj[a].dest->info == current->info)
						break;
				}
				if (a < reverseVertex->adj.size())
				{
					float prevValue = reverseVertex->adj[a].flow;

					reverseVertex->updateEdgeFlow(a, prevValue - newmin);
					Edge<T> reverseEdge = reverseVertex->adj[a];

					cout << "*** ADJUSTING edge flow value to the final graph from " << reverseVertex->info << " to " << reverseEdge.dest->info << " with a flow value of " << reverseEdge.flow << endl;
				}

			}

			cout << "*** Returning new minimum value of " << newmin << " after recursion!" << endl;
			return newmin;
		}

	}

	//Return the previously found minimum?
	return -1;
}

#endif /* GRAPH_H_ */
