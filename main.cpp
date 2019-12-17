#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

//----------------------------------------------------------------------------------------------------------------------------------
//----Variaveis Globais-------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

double alfa = 0.5;
double torneio = 0.1;

//----------------------------------------------------------------------------------------------------------------------------------
//----Estruturas--------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------

struct Individuo
{
    double crom;
    double fitness;
};

//----------------------------------------------------------------------------------------------------------------------------------
//----Cabeçalho da Funções----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------


// Fução que gera executa o algoritmo genetico pelo numero de gerações especificados e printa a geração final e o melhor individuo.
void gen(uint32_t tam_pop, 
         uint32_t max_gen, 
         double prob_cruz, 
         double prob_mut, 
         bool elitismo);


// Função que calcula o fitness do individuo, baseado no seu cromossomo
double func_obj(double x);

// Função que inicia a população com valores do cromossomo variando de [0, 500]
Individuo *inic_pop(uint32_t tam_pop);

// Função que seleciona os dois melhores inidividuos de um subgrupo da população, esse subgrupo é determinado pela variavel
// global torneio.
std::pair<uint32_t, uint32_t> selecao(Individuo *pop, 
                                      uint32_t tam_pop);

// Função que seleciona o melhor inidividuo de um subgrupo da população, esse subgrupo é determinado pela variavel global 
// torneio.
uint32_t alt_selecao(Individuo *pop, 
                     uint32_t tam_pop);


// Função que gera a proxima geração de inidividuos
Individuo *geracao(Individuo *pop_atual, 
                   uint32_t tam_pop, 
                   double p_cruz, 
                   double p_mut, 
                   bool elitismo);

// Função que gera um valor aleatorio entre [-alfa, 1 + alfa], sendo alfa a variavel global alfa.
double random_number();

// Função que retorna o melhor individuo da população.
Individuo melhor_individuo(Individuo *pop, 
                           uint32_t tam_pop);

// Função que pode gerar uma mutação no individuo, isso depende do p_mut que determina a chance de gerar uma mutação
void mutacao(Individuo &ind, 
             double p_mut);

// Função que faz o cruzamento de dois inidividuos, gerando dois novos inidividous para a proximação população.
std::pair<Individuo, Individuo> cruzamento(Individuo &pai1, Individuo &pai2, double p_cruz, double p_mut, bool elitismo);

//---------------------------------------------------------------------------------------------------------------------------------
//----Implementação----------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

void imprimi_pop(Individuo *pop,uint32_t tam_pop)
{
    for(uint32_t i =0; i< tam_pop; i++)
    {
        std::cout << pop[i].crom << " | " << pop[i].fitness << std::endl;
    }
    std::cout << std::endl;
}


void gen(uint32_t tam_pop, uint32_t max_gen, double prob_cruz, double prob_mut, bool elitismo)
{
    uint32_t ger = 0;  
    Individuo *pop = inic_pop(tam_pop);
    for(uint32_t i =0; i < max_gen; i++)
    {
        pop = geracao(pop, tam_pop, prob_cruz, prob_mut, elitismo);
    }
    std::cout << "Ultima Geração:" << std::endl;
    std::cout << "Cromossomo" << " | " << "Fitness" << std::endl;
    std::cout << "--------------------" << std::endl;

    if(tam_pop < 250)
        imprimi_pop(pop, tam_pop);

    Individuo melhor = melhor_individuo(pop, tam_pop);
    std::cout << "Melhor Individuo é:" << std::endl;

    std::cout << "Cromossomo: " << melhor.crom << std::endl;
    std::cout << "Fitness: " << melhor.fitness << std::endl;

    std::cout << "-------------------------------" << std::endl; 
    delete[] pop;
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
        populacao[i].crom = (double)(rand()%5000)/10;
        populacao[i].fitness = func_obj(populacao[i].crom);
    }
    return populacao;
}

Individuo melhor_individuo(Individuo *pop, uint32_t tam_pop)
{
    Individuo melhor = pop[0];
    for(uint32_t i =1; i < tam_pop; i++)
    {
        if(melhor.fitness > pop[i].fitness)
            melhor = pop[i];
    }
    return melhor;
}

std::pair<uint32_t, uint32_t> selecao(Individuo *pop, uint32_t tam_pop)
{
    uint32_t num_elementos = tam_pop * torneio;
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
    return std::pair<uint32_t, uint32_t> (menor.first, vice_menor.first);
}

uint32_t alt_selecao(Individuo *pop, uint32_t tam_pop)
{
    uint32_t num_elementos = tam_pop * torneio;
    std::pair<uint32_t, double> menor;

    menor.first = rand()%num_elementos;
    menor.second = pop[menor.first].fitness;

    for(uint32_t i = 1; i < num_elementos; i++)
    {
        uint32_t novo = rand()%num_elementos;
        if(pop[novo].fitness < menor.second)
        {
            menor.first = novo;
            menor.second = pop[novo].fitness;
        }
    }

    return menor.first;
}

Individuo *geracao(Individuo *pop_atual, uint32_t tam_pop, double p_cruz, double p_mut, bool elitismo)
{
    Individuo *pop_nova = new Individuo[tam_pop];
    for(uint32_t j=0; j<tam_pop; j+=2)
    {
        uint32_t ind1 = alt_selecao(pop_atual, tam_pop);
        uint32_t ind2 = alt_selecao(pop_atual, tam_pop);
        std::pair<Individuo, Individuo> filhos = cruzamento(pop_atual[ind1], pop_atual[ind2], p_cruz, p_mut, elitismo);

        pop_nova[j] = filhos.first;
        pop_nova[j+1] = filhos.second; 
    }
    delete[] pop_atual;
    return pop_nova;
}

double random_number()
{
    int range = (alfa*100)*2 + 100;
    double beta = (rand()%range)/100.0 - alfa;
    return beta;
}

void mutacao(Individuo &ind, double p_mut)
{
    double mutation = (double)rand()/RAND_MAX;
    if(mutation > p_mut)
        return;
    else
    {
        ind.crom = (double)(rand()%5000)/10;
        ind.fitness = func_obj(ind.crom);
    }
    return;
}

std::pair<Individuo, Individuo> cruzamento(Individuo &pai1, Individuo &pai2, double p_cruz, double p_mut, bool elitismo)
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
    if(elitismo)
    {
        Individuo melhor = pai1;
        if(pai2.fitness > melhor.fitness)
            melhor = pai2;
        if(melhor.fitness < filhos.first.fitness)
        {
            filhos.first = melhor;
        }
        if(melhor.fitness < filhos.second.fitness)
        {
            filhos.second = melhor;
        }
    }
    return filhos;
}

//---------------------------------------------------------------------------------------------------------------------------------
//----Execução---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

int main()
{
    srand(time(NULL));

    bool sem_menu = true;

    uint32_t tam_pop = 100;
    uint32_t max_gen = 1000;
    double prob_cruz = 0.8;
    double prob_mut = 0.05;
    bool elitismo = false;

    if(sem_menu)
    {
        gen(tam_pop,max_gen,prob_cruz,prob_mut,elitismo);
        return 0;
    }

    std::cout << "Bem vindo!" << std::endl;

    std::cout << "Quantos individuos terao em cada geracao: ";
    std::cin >> tam_pop;

    std::cout << "Quantas serao as geracoes: ";
    std::cin >> max_gen;

    std::cout << "Quanto sera a taxa de cruzamento: ";
    std::cin >> prob_cruz;

    std::cout << "Quanto sera a taxa de mutacao: ";
    std::cin >> prob_mut;

    std::cout << "Quanto os filhos poderam ser diferentes de seus pais (range): ";
    std::cin >> alfa;

    std::cout << "Qual a porcentagem de individuos que irao para o torneio: ";
    std::cin >> torneio;

    gen(tam_pop,max_gen,prob_cruz,prob_mut,elitismo);
}