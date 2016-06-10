#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "anagrama.h"
#include <iomanip>
using namespace std;

class InputError{};

int getDistance_old(string pString, string tString)
{
	int dMatrix[50][50],i,j;
	pString = "@"+pString;
	tString = "@"+tString;
	//inicializacao
	for(i = 0; i < pString.size(); i++) dMatrix[i][0] = i;
	for(j = 0; j < tString.size(); j++) dMatrix[0][j] = j;

	//recorrencia
	for(i = 1; i < pString.size(); i++)
	{
		for(j= 1; j < tString.size(); j++)
		{
			if (pString.at(i) == tString.at(j)) dMatrix[i][j] = dMatrix[i-1][j-1];
			else
			{
				dMatrix[i][j] = 1 + min(dMatrix[i-1][j-1], min(dMatrix[i-1][j],dMatrix[i][j-1]));
			}
		}
	}


	return dMatrix[pString.size()-1][tString.size()-1];

}

double getSemelhanca(string pString, string tString, int distancia)
{
	return 100*((double)(pString.size()-distancia)/pString.size());

}

int getDistance(string pString, string tString)
{
	int dArray[50], i, j, old_value, new_value;
	pString = "@"+pString;
	tString = "@"+tString;

	for(j= 0; j < tString.size(); j++) dArray[j] = j;

	for(i = 1; i < pString.size(); i++)
	{
		old_value = dArray[0];
		dArray[0] = i;
		for(j = 1; j < tString.size(); j++)
		{
			if (pString.at(i) == tString.at(j))
			{
				new_value = old_value;
			}
			else
			{
				new_value = 1 + min(old_value, min(dArray[j],dArray[j-1]));
			}

			old_value = dArray[j];
			dArray[j] = new_value;
		}
	}

	return(dArray[tString.size()-1]);
}



int loadDic(string filename, vector< vector<string> >& output)
{
	int dic_size = -1;
	ifstream infile(filename.c_str());
	string temp;

	getline(infile,temp);
	dic_size = atoi(temp.c_str());
	while(!infile.eof())
	{
		getline(infile,temp);
		output.at(temp.size()).push_back(temp);
	}

	infile.close();

	return dic_size;

}


void anagramaSimples(vector< vector<string> > dicionario)
{
	vector<Anagrama> resultado;
	Anagrama palavraTemp;
	//vector<string> resultado;
	string inputWord, seedWord, dicWord, comparableWord, loweredWord, loweredDicWord;

	cout << "Insira a palavra geradora de anagramas: " << endl;
	cin>>inputWord;
	seedWord = inputWord; //COPIAR PARA NAO PERDER ORIGINAL
	transform(seedWord.begin(), seedWord.end(), seedWord.begin(), ::tolower); //CONVERTER LOWERCASE
	loweredWord = seedWord;
	sort(seedWord.begin(),seedWord.end()); // ORDENAR PALAVRA
	

	for(unsigned int i = 0; i < dicionario.at(seedWord.size()).size(); i++)
	{
		dicWord = dicionario.at(seedWord.size()).at(i);

		comparableWord = dicWord; //COPIAR PARA NAO PERDER ORIGINAL
		transform(comparableWord.begin(), comparableWord.end(), comparableWord.begin(), ::tolower); //CONVERTER LOWERCASE
		loweredDicWord = comparableWord;
		sort(comparableWord.begin(), comparableWord.end()); // ORDENAR PALAVRA
		

		if (comparableWord == seedWord)
		{
			palavraTemp.setPalavra(dicWord);
			palavraTemp.setDistEdicao(getDistance(loweredDicWord, loweredWord));
			//palavraTemp.setSemelhanca(100*(double)(inputWord.size() - palavraTemp.getDistEdicao()) /inputWord.size() );
			palavraTemp.setSemelhanca(getSemelhanca(loweredWord, loweredDicWord, palavraTemp.getDistEdicao()));
			resultado.push_back(palavraTemp); //SE FOR IGUAL ADICIONA VETOR RESULTADOS
		}
	}
	
	sort(resultado.begin(),resultado.end());
	cout << endl << "As Palavras resultado sao: " << endl;
	for(unsigned int i = 0; i < resultado.size(); i++)
	{
		//cout << resultado.at(i) << " Distancia1: " << getDistance(resultado.at(i),inputWord ) << endl; //" " << getDistance2(resultado.at(i),inputWord ) <<  endl;
		cout << left << setw(10) << resultado.at(i).getPalavra() << " Distancia: " << setw(5) << resultado.at(i).getDistEdicao() << " Semelhanca: " << setw(5) << resultado.at(i).getSemelhanca() << "%" << endl;
	}

	

	cout << endl << "Clique para voltar ao menu inicial" << endl;
	system("pause");
}


void anagramaComplexo(vector< vector<string> > dicionario)
{
	int distPrimeira, distSegunda, wordSize, nLetrasIguais;
	//vector<string> resultado, resultadoINF, resultadoSUP;
	vector<Anagrama>resultado;
	Anagrama palavraTemp;
	string inputWord, seedWord, dicWord, comparableWord, loweredWord, loweredDicWord;

	cout << "Insira a palavra geradora de anagramas: " << endl;
	cin>>inputWord;
	seedWord = inputWord; //COPIAR PARA NAO PERDER ORIGINAL
	transform(seedWord.begin(), seedWord.end(), seedWord.begin(), ::tolower); //CONVERTER LOWERCASE
	loweredWord = seedWord;
	sort(seedWord.begin(),seedWord.end()); // ORDENAR PALAVRA
	

	// PERCORRER VECTOR MESMO TAMANHO + ALTERACAO DE UMA LETRA
	
	for(unsigned int i = 0; i < dicionario.at(seedWord.size()).size(); i++)
	{
		distPrimeira = 0;
		distSegunda = 0;

		dicWord = dicionario.at(seedWord.size()).at(i);
		comparableWord = dicWord; //COPIAR PARA NAO PERDER ORIGINAL
		transform(comparableWord.begin(), comparableWord.end(), comparableWord.begin(), ::tolower); //CONVERTER LOWERCASE
		loweredDicWord = comparableWord;
		sort(comparableWord.begin(), comparableWord.end()); // ORDENAR PALAVRA
		wordSize = comparableWord.size();
		

		if (comparableWord == seedWord)
		{
			palavraTemp.setPalavra(dicWord);
			palavraTemp.setDistEdicao(getDistance(loweredDicWord, loweredWord));
			palavraTemp.setSemelhanca(getSemelhanca(loweredWord, loweredDicWord, palavraTemp.getDistEdicao()));
			resultado.push_back(palavraTemp); //SE FOR IGUAL ADICIONA VETOR RESULTADOS
			//resultado.push_back(dicWord); //SE FOR IGUAL ADICIONA VETOR RESULTADOS
		}
		else
		{
			for(int j = 0; j < wordSize; j++)
			{
				if (comparableWord.at(j) != seedWord.at(j)) distPrimeira++;
			}
			if (distPrimeira == 1)
			{
				palavraTemp.setPalavra(dicWord);
				palavraTemp.setDistEdicao(getDistance(dicWord, inputWord));
				palavraTemp.setSemelhanca(getSemelhanca(inputWord, dicWord, palavraTemp.getDistEdicao()));
				resultado.push_back(palavraTemp); //SE FOR IGUAL ADICIONA VETOR RESULTADOS
			}

		}
	}
	

	// PERCORRER VECTOR UMA UNIDADE INFERIOR

	/*for(unsigned int i = 0; i < dicionario.at(seedWord.size() - 1).size(); i++)
	{
		distPrimeira = 0;
		distSegunda = 0;
		nLetrasIguais = 0;

		dicWord = dicionario.at(seedWord.size()-1).at(i);
		comparableWord = dicWord; //COPIAR PARA NAO PERDER ORIGINAL
		transform(comparableWord.begin(), comparableWord.end(), comparableWord.begin(), ::tolower); //CONVERTER LOWERCASE
		sort(comparableWord.begin(), comparableWord.end()); // ORDENAR PALAVRA
		wordSize = comparableWord.size();
		

		for(int j = 0; j < seedWord.size(); j++)
		{
			for(int k = 0; k < wordSize; k++)
			{
				if (seedWord.at(j) == comparableWord.at(k))
				{
					nLetrasIguais++;
					comparableWord.at(k)= '#' ;
					break;
				}
			}
		}

		if (nLetrasIguais == comparableWord.size())
		{
			palavraTemp.setPalavra(dicWord);
			palavraTemp.setDistEdicao(getDistance(dicWord, inputWord));
			//palavraTemp.setSemelhanca(100*(double)(inputWord.size() - palavraTemp.getDistEdicao()) /inputWord.size() );
			palavraTemp.setSemelhanca(getSemelhanca(inputWord, dicWord, palavraTemp.getDistEdicao()));
			resultado.push_back(palavraTemp); //SE FOR IGUAL ADICIONA VETOR RESULTADOS	
		}
	}*/
	

	// PERCORRER VECTOR UMA UNIDADE SUPERIOR

	for(unsigned int i = 0; i < dicionario.at(seedWord.size() + 1).size(); i++)
	{
		
		distPrimeira = 0;
		distSegunda = 0;
		nLetrasIguais = 0;

		dicWord = dicionario.at(seedWord.size()+1).at(i);
		comparableWord = dicWord; //COPIAR PARA NAO PERDER ORIGINAL
		transform(comparableWord.begin(), comparableWord.end(), comparableWord.begin(), ::tolower); //CONVERTER LOWERCASE
		sort(comparableWord.begin(), comparableWord.end()); // ORDENAR PALAVRA
		wordSize = comparableWord.size();
		//cout << "Entrou SUPERIOR. PALAVRA: " << dicWord << endl; 
		//system("Pause");
		

		for(int j = 0; j < wordSize; j++)
		{
			for(int k = 0; k < seedWord.size(); k++)
			{
				if (comparableWord.at(j) == seedWord.at(k))
				{
					nLetrasIguais++;
					seedWord.at(k)= '#' ;
					break;
				}
			}
		}

		if (nLetrasIguais == seedWord.size())
		{
			palavraTemp.setPalavra(dicWord);
			palavraTemp.setDistEdicao(getDistance(dicWord, inputWord));
			//palavraTemp.setSemelhanca(100*(double)(inputWord.size() - palavraTemp.getDistEdicao()) /inputWord.size() );
			palavraTemp.setSemelhanca(getSemelhanca(inputWord, dicWord, palavraTemp.getDistEdicao()));
			resultado.push_back(palavraTemp); //SE FOR IGUAL ADICIONA VETOR RESULTADOS
		}
	}
	
	sort(resultado.begin(),resultado.end());
	cout << endl << "As Palavras resultado " <<resultado.size() << "  sao: " << endl;
	for(unsigned int i = 0; i < resultado.size(); i++)
	{
		//cout << resultadoSUP.at(i) << endl;
		cout << left << setw(10) << resultado.at(i).getPalavra() << " Distancia: " << setw(5) << resultado.at(i).getDistEdicao() << " Semelhanca: " << setw(5) << resultado.at(i).getSemelhanca() << "%" << endl;
	}

	

	

	cout << endl << "Clique para voltar ao menu inicial" << endl;
	system("pause");
}


int main()
{
	string filename = "dicionario.txt";
	vector< vector<string> >dicionario (30);
	int i = 0, menuOption = 0;

	cout << loadDic(filename,dicionario) << endl;

	while(menuOption != 3 )
	{
		try
		{

			system("cls");
			cout << "Gerador de Anagramas" << endl;
			cout << "1 - Anagramas Simples " << endl;
			cout << "2 - Anagramas Complexos (Troca de Letras)" << endl;
			cout << "3 - Sair" << endl;
			cin >> menuOption;
			if (cin.fail()) throw InputError();

			switch(menuOption)
			{
				case 1: anagramaSimples(dicionario);
						break;
				case 2:	anagramaComplexo(dicionario);
						break;

				case 3: cout << "O Programa Vai Terminar" << endl;
						system("pause");
						break;

				default: cout << "Opcao Invalida" << endl;
						system("pause");
						break;
			}
		}
		catch(InputError)
		{
			cout << "Input Invalido! " << endl;
			system("pause");
			cin.clear();
			cin.ignore(1000,'\n');
		}

	}

	return 0;

}

