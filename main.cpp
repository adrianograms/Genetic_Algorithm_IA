#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------------------------
//----Estruturas---------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

struct Individuo
{
    double crom;
    double fitness;
};


//----------------------------------------------------------------------------------------------------------------------------------
//----Cabeçalho---------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

void gen(uint32_t tam_pop, uint32_t max_gen, double prob_cruz, double prob_mut, uint32_t lim_inf, uint32_t lim_sup);

//---------------------------------------------------------------------------------------------------------------------------------
//----Implementação----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

void gen(uint32_t tam_pop, uint32_t max_gen, double prob_cruz, double prob_mut, uint32_t lim_inf, uint32_t lim_sup)
{
    uint32_t ger = 0;
}

Individuo *inic_pop(uint32_t tam_pop, uint32_t li, uint32_t ls)
{
    Individuo *populacao = new Individuo[tam_pop];
    srand(time(0));
    for(uint32_t i = 0; i< tam_pop; i++)
    {
        populacao[i].crom = rand();
        populacao[i].fitness = func_obj(populacao[i].crom);
    }
    return populacao;
}

double min_fitness(Individuo *pop, uint32_t tam_pop)
{
    double menor = pop[0].fitness;
    for(uint32_t i = 1; i < tam_pop; i++)
    {
        if(menor > pop[i].fitness)
            menor = pop[i].fitness;
    }
    return menor;
}

double media_fitness(Individuo *pop, uint32_t tam_pop)
{
    double media = 0.0;
    for(uint32_t i =0; i< tam_pop; i++)
    {
        media += pop[i].fitness;
    }
    return media/tam_pop;
}

double func_obj(double x)
{
    return (864/x) + std::pow(x, 2);
}

int main()
{
}