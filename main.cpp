#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define ALFA 0.5
#define TORNEIO 0.1

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

void gen(uint32_t tam_pop, uint32_t max_gen, double prob_cruz, double prob_mut);
double func_obj(double x);
Individuo *inic_pop(uint32_t tam_pop);
double min_fitness(Individuo *pop, uint32_t tam_pop);
double media_fitness(Individuo *pop, uint32_t tam_pop);
double soma_fitness(Individuo *pop, uint32_t tam_pop);
//uint32_t selecao(Individuo *pop, uint32_t tam_pop, double soma_fit);
std::pair<uint32_t, uint32_t> selecao(Individuo *pop, uint32_t tam_pop, double soma_fit);
Individuo *geracao(Individuo *pop_atual, uint32_t tam_pop, double p_cruz, double p_mut);
double random_number();
void mutacao(Individuo &ind, double p_mut);
std::pair<Individuo, Individuo> cruzamento(Individuo &pai1, Individuo &pai2, double p_cruz, double p_mut);

//---------------------------------------------------------------------------------------------------------------------------------
//----Implementação----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
void imprimi_pop(Individuo *pop,uint32_t tam_pop)
{
    for(uint32_t i =0; i< tam_pop; i++)
    {
        std::cout << pop[i].crom << "," << pop[i].fitness << std::endl;
    }
    std::cout << std::endl;
}


void gen(uint32_t tam_pop, uint32_t max_gen, double prob_cruz, double prob_mut)
{
    uint32_t ger = 0;  
    Individuo *pop = inic_pop(tam_pop);
    for(uint32_t i =0; i < max_gen; i++)
    {
        pop = geracao(pop, tam_pop, prob_cruz, prob_mut);
    }
    imprimi_pop(pop, tam_pop);
}

double func_obj(double x)
{
    if(x > 0.0)
        return (864/x) + std::pow(x, 2);
    else
        return MAXFLOAT;
    
}

Individuo *inic_pop(uint32_t tam_pop)
{
    Individuo *populacao = new Individuo[tam_pop];
    for(uint32_t i = 0; i< tam_pop; i++)
    {
        populacao[i].crom = rand()%500;
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

double soma_fitness(Individuo *pop, uint32_t tam_pop)
{
    double soma = 0.0;
    for(uint32_t i=0; i<tam_pop; i++)
    {
        if(pop[i].fitness != 0.0)
            soma += (1/pop[i].fitness);
    }
    return soma;
}

// uint32_t selecao(Individuo *pop, uint32_t tam_pop, double soma_fit)
// {
//     double soma_parte = 0.0;
//     double rnd = ((double)(rand()%10000)/10000.0)*soma_fit;
//     std::cout << rnd << std::endl;
//     uint32_t j;
//     for(j = 0; j < tam_pop && soma_parte < rnd;)
//     {
//         soma_parte += (1/pop[j].fitness);
//         j++;
//     }
//     return j--;
    
// }

std::pair<uint32_t, uint32_t> selecao(Individuo *pop, uint32_t tam_pop, double soma_fit)
{
    uint32_t num_elementos = tam_pop * TORNEIO;
    std::pair<uint32_t, double> escolhidos[num_elementos];
    std::pair<uint32_t, double> menor, vice_menor;
    escolhidos[0].first = rand()%num_elementos;
    escolhidos[0].second = pop[escolhidos[0].first].fitness;
    menor = escolhidos[0];
    for(uint32_t i = 1; i< num_elementos; i++)
    {
        escolhidos[i].first = rand()%num_elementos;
        escolhidos[i].second = pop[escolhidos[i].first].fitness;

        if(escolhidos[i].second < menor.second)
            menor = escolhidos[i];
    }
    vice_menor.first = 0;
    vice_menor.second = MAXFLOAT;
    for(uint32_t i = 0; i < num_elementos; i++)
    {
        if(escolhidos[i].first != menor.first)
        {
            if(escolhidos[i].second <= vice_menor.second)
            {
                vice_menor = escolhidos[i];
            }
        }
    }
    //delete[] escolhidos;
    return std::pair<uint32_t, uint32_t> (menor.first, vice_menor.first);
}

Individuo *geracao(Individuo *pop_atual, uint32_t tam_pop, double p_cruz, double p_mut)
{
    Individuo *pop_nova = new Individuo[tam_pop];
    double sf = soma_fitness(pop_atual, tam_pop);
    for(uint32_t j=0; j<tam_pop; j+=2)
    {
        //uint32_t ind1 = selecao(pop_atual, tam_pop, sf);
        //uint32_t ind2 = selecao(pop_atual, tam_pop, sf);

        std::pair<uint32_t, uint32_t> escolhidos = selecao(pop_atual, tam_pop, sf);

        std::pair<Individuo, Individuo> filhos = cruzamento(pop_atual[escolhidos.first], pop_atual[escolhidos.second], p_cruz, p_mut);
        pop_nova[j] = filhos.first;
        pop_nova[j+1] = filhos.second; 
    }
    delete[] pop_atual;
    return pop_nova;
}

double random_number()
{
    int random_number = rand() - (int)(RAND_MAX/2);
    double beta = (random_number%(int)(ALFA*100))/100.0;
    return beta;
}

void mutacao(Individuo &ind, double p_mut)
{
    double mutation = (double)rand()/RAND_MAX;
    if(mutation > p_mut)
        return;
    else
    {
        ind.crom = rand()%500;
        ind.fitness = func_obj(ind.crom);
    }
    return;
    
}

std::pair<Individuo, Individuo> cruzamento(Individuo &pai1, Individuo &pai2, double p_cruz, double p_mut)
{
    std::pair<Individuo, Individuo> filhos;

    double crossover = (double)rand()/RAND_MAX;
    if(crossover > p_cruz)
    {
        filhos.first.crom = pai1.crom;
        filhos.first.fitness = pai1.fitness;

        filhos.second.crom = pai2.crom;
        filhos.second.fitness = pai2.fitness;
    }
    else
    {
        double beta1 = random_number();
        double beta2 = random_number();
        double maior, menor;
        if(pai1.crom > pai2.crom)
        {
            maior = pai1.crom;
            menor = pai2.crom;
        }
        else
        {
            maior = pai2.crom;
            menor = pai1.crom;
        }
        
        filhos.first.crom = menor + beta1*(maior - menor);
        filhos.second.crom = menor + beta2*(maior - menor);

        filhos.first.fitness = func_obj(filhos.first.crom);
        filhos.second.fitness = func_obj(filhos.second.crom);
    }
    mutacao(filhos.first, p_mut);
    mutacao(filhos.second, p_mut);
    return filhos;
}


int main()
{
    srand(time(NULL));
    gen(50,1000,0.8,0.05);
}