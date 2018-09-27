#include <iostream>
#include <cstring>
#include <cstdlib>
#include "dna.h"
#include <vector>
#include <ctime>
#include <algorithm>

////// Define Utils

#define TAM_POP 5000
#define LIM_IT 10000

// chance de mutação = MUT_CHANCE
#define MUT_CHANCE 1

///////////////////




using namespace std;

void roleta(int & pai, int & mae)
{
  pai = rand()%TAM_POP;
  mae = rand()%TAM_POP;
}


int main(int argc, char** argv)
{
  srand(time(NULL));

  double mut_c = 0.02;
  vector<DNA*> populacao;
  int totalFit = 0 ;

  string objetivo(argv[1]);
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

  while(it < LIM_IT)
  {
    // SELECAO
    totalFit = 0;

    for(int i = 0; i < TAM_POP; i++){
      populacao.at(i)->fit = 0;
      for(int j = 0; j < tam_objetivo; j++){
        if(populacao.at(i)->gene.at(j) == objetivo.at(j)){
          populacao.at(i)->fit += 10;
        }
      }
      if(populacao.at(i)->fit == tam_objetivo * 10) {
        cout << "PALAVRA ENCONTRADA NA GERACAO: " << it << endl;
        return 0;
      }
      totalFit += populacao.at(i)->fit;
    }

    // ORDENAR PELO FIT

    sort(populacao.begin(), populacao.end(), [](DNA * a, DNA *b) {
      return a->fit > b->fit;
    });

    if(it %10 == 0)
    cout << it << " MELHOR ORGANISMO DA GERACAO: " << populacao.at(0)->gene << endl;

    // ROLETA
/*    vector<double> probabilidade_roleta(TAM_POP + 1, 0);

    for(int i = 1; i <= TAM_POP; i++) {
      probabilidade_roleta[i] = ((double)populacao.at(i-1)->fit/(double)totalFit) + probabilidade_roleta[i-1];
    }
*/


    // CROSSOVER
    for(int i = 0; i < TAM_POP; i++){

      int pai = -1, mae = -1;

      roleta(pai, mae);

      string auxPai, auxMae;

      for(int j = 0; j < tam_objetivo/2; j++) {
        (((double) rand()/ (RAND_MAX)) <= mut_c)? auxPai += rand()%96 + 32 : auxPai += populacao.at(pai)->gene.at(j);
        (((double) rand()/ (RAND_MAX)) <= mut_c)? auxMae += rand()%96 + 32 : auxMae += populacao.at(pai)->gene.at(j);
      }
      for(int j = tam_objetivo/2; j < tam_objetivo; j++) {
        (((double) rand()/ (RAND_MAX)) <= mut_c)? auxPai += rand()%96 + 32 : auxPai += populacao.at(mae)->gene.at(j);
        (((double) rand()/ (RAND_MAX)) <= mut_c)? auxMae += rand()%96 + 32 : auxMae += populacao.at(mae)->gene.at(j);
      }

      populacao.at(pai)->gene = auxPai;
      populacao.at(mae)->gene = auxMae;
    }

    it++;
  }

  return 0;
}
