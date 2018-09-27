/*
  Authors:
            - Felipe Oliveira Lins e Silva
            - Luis Gabriel Pereira Condados
*/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "dna.h"
#include <vector>
#include <ctime>
#include <algorithm>
#include <queue>

////// Define Utils
#define TAM_POP 50000
#define MAX_GERACOES 10000
#define MUT_CHANCE 0.02L
///////////////////


using namespace std;

void fitness(DNA* a,  const string& objetivo);
void crossoverEMutacao(int & pai, int & mae, queue<DNA*> &newPop, vector<DNA*> & populacao, int & tam_objetivo);


int main(int argc, char** argv)
{
  srand(time(NULL));


  vector<DNA*> populacao;
  int totalFit = 0 ;

  string objetivo("HIGAR");
  int tam_objetivo = objetivo.length();

  // inicializar populacao
  for(int i = 0; i < TAM_POP; i++) {
    populacao.push_back(new DNA());
    populacao.at(i)->gene.resize(tam_objetivo);

    // preencher a populacao inicial com coisa aleatoria
    for(int j = 0; j < tam_objetivo; j++) {
      populacao.at(i)->gene.at(j) = rand()%96 + 32;
    }
  }

  int it = 0;

  while(it < MAX_GERACOES)
  {
    // SELECAO
    totalFit = 0;

    for(int i = 0; i < TAM_POP; i++){
      populacao.at(i)->fit = 0;
      fitness(populacao.at(i),objetivo);

      if(populacao.at(i)->fit == tam_objetivo * 10) {
        cout << "PALAVRA ENCONTRADA NA GERACAO: " << it << endl;
        cout << "PALAVRA: " << populacao.at(i)->gene << endl;
        return 0;
      }
      totalFit += populacao.at(i)->fit;
    }

    // ORDENAR PELO FIT
    sort(populacao.begin(), populacao.end(), [](DNA *& a, DNA *& b) {
      return a->fit > b->fit;
    });

    cout << "GERACAO: " << it << " | MELHOR ORGANISMO DA GERACAO: " << populacao.at(0)->gene << endl;

    // nova populacao
    queue<DNA*> newPop;

    // CROSSOVER
    int pai = 0; // MELHOR FIT

    for(int i = 0; i < TAM_POP/2; i++){
      int mae = rand() % TAM_POP;
      crossoverEMutacao(pai, mae, newPop, populacao, tam_objetivo);
    }

    // renovar populacao
    for(int i = 0; i < TAM_POP; i++) {
      delete populacao[i];
    }
    for(int i = 0; i < TAM_POP; i++) {
      populacao[i] = newPop.front();
      newPop.pop();
    }


    it++;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void fitness(DNA* a,  const string& objetivo){
  for(uint i = 0; i < objetivo.length(); i++)
    if(a->gene.at(i) == objetivo.at(i))
      a->fit += 10;
}

void crossoverEMutacao(int & pai, int & mae, queue<DNA*> &newPop, vector<DNA*> & populacao, int & tam_objetivo)
{
  string auxPai, auxMae;

  for(int j = 0; j < tam_objetivo/2; j++) {
    (((double) rand()/ (RAND_MAX)) <= MUT_CHANCE)? auxPai += rand()%96 + 32 : auxPai += populacao.at(pai)->gene.at(j);
    (((double) rand()/ (RAND_MAX)) <= MUT_CHANCE)? auxMae += rand()%96 + 32 : auxMae += populacao.at(pai)->gene.at(j);
  }
  for(int j = tam_objetivo/2; j < tam_objetivo; j++) {
    (((double) rand()/ (RAND_MAX)) <= MUT_CHANCE)? auxPai += rand()%96 + 32 : auxPai += populacao.at(mae)->gene.at(j);
    (((double) rand()/ (RAND_MAX)) <= MUT_CHANCE)? auxMae += rand()%96 + 32 : auxMae += populacao.at(mae)->gene.at(j);
  }

  DNA* tmp1 = new DNA();
  DNA* tmp2 = new DNA();

  tmp1->gene = auxPai;
  tmp2->gene = auxMae;

  newPop.push(tmp1);
  newPop.push(tmp2);
}
