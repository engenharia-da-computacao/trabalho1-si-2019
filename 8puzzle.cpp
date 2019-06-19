#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <math.h>
#include <chrono>
#include <algorithm>

using namespace std;

/** Inicialização **/
struct listMatriz
{

    vector<vector<int>> puzzle;
};

struct Statics
{
    double time;
    int movs;
};

bool algoritmoGuloso(vector<vector<int>> matriz);

bool buscaEmProfundidade(vector<vector<int>> matriz);
bool matrizVisitada(int matrizInt);
vector<listMatriz> retornaFilhos(vector<vector<int>> matriz);
void addVisitados(vector<vector<int>> matriz);
int converteMatrizInt(vector<vector<int>> matriz);
void printMatriz(vector<vector<int>> matriz);
void imprimeEstatisticas();

/** Variável global de visitados **/
vector<long double> puzzleslVisitadas;

void clearVisitados() { puzzleslVisitadas.clear(); }

/** Representa a forma final do puzzle convertido em inteiro **/
constexpr int puzzleFinal{12345678};

int finalPosition[9][2];

int main()
{
    /** Setando posições finais **/
    finalPosition[0][0] = 2;
    finalPosition[0][1] = 2;
    finalPosition[1][0] = 0;
    finalPosition[1][1] = 0;
    finalPosition[2][0] = 0;
    finalPosition[2][1] = 1;
    finalPosition[3][0] = 0;
    finalPosition[3][1] = 2;
    finalPosition[4][0] = 1;
    finalPosition[4][1] = 0;
    finalPosition[5][0] = 1;
    finalPosition[5][1] = 1;
    finalPosition[6][0] = 1;
    finalPosition[6][1] = 2;
    finalPosition[7][0] = 2;
    finalPosition[7][1] = 0;
    finalPosition[8][0] = 2;
    finalPosition[8][1] = 1;

    /** Estatisticas **/
    Statics stBT[5];
    Statics stG[5];

    vector<listMatriz> instancias;
    vector<vector<int>> puzzleEntrada;
    instancias.push_back({{{1, 2, 0}, {3, 4, 5}, {6, 7, 8}}});
    instancias.push_back({{{1, 2, 5}, {0, 3, 4}, {6, 7, 8}}});
    instancias.push_back({{{1, 2, 5}, {6, 3, 4}, {0, 7, 8}}});
    instancias.push_back({{{1, 2, 5}, {6, 3, 4}, {7, 0, 8}}});
    //instancias.push_back({ { {1,2,5}, {6,0,8}, {3,7,4} } });
    instancias.push_back({{{1, 2, 5}, {3, 4, 0}, {6, 7, 8}}});
    //auto startTime, endTime, diff;
    int numInstancias = instancias.size(), i;

    for (i = 0; i < numInstancias; i++)
    {
        clearVisitados();

        puzzleEntrada = instancias[i].puzzle;
        /** Start Processing BackTracking **/
        auto startTime = chrono::steady_clock::now();
        buscaEmProfundidade(puzzleEntrada);

        auto endTime = chrono::steady_clock::now();
        auto diff = endTime - startTime;

        stBT[i].time = chrono::duration<double, milli>(diff).count();
        stBT[i].movs = puzzleslVisitadas.size();

        clearVisitados();
        /** Start Processing Guloso **/
        startTime = chrono::steady_clock::now();
        algoritmoGuloso(puzzleEntrada);

        endTime = chrono::steady_clock::now();
        diff = endTime - startTime;

        stG[i].time = chrono::duration<double, milli>(diff).count();
        stG[i].movs = puzzleslVisitadas.size();
    }

    cout << "\n********************* Estatisticas *********************\n";
    for (i = 0; i < numInstancias; i++)
    {
        /** Exibindo **/
        cout << "\nEstatisticas busca em profundidade\n";
        cout << "->Instancia [" << i + 1 << "] \n";
        cout << "\n->Números de movimentos: " << stBT[i].movs << "\n";
        cout << "\n->Tempo gasto: " << stBT[i].time << " ms \n";

        cout << "\n\nEstatisticas Guloso";
        cout << "\n->Instancia [" << i + 1 << "] \n";
        cout << "\n->Números de movimentos: " << stG[i].movs << "\n";
        cout << "\n->Tempo gasto: " << stG[i].time << " ms \n";
        cout << "\n\n\n";
    }

    cin.get();
}

int calculaN(vector<vector<int>> matriz)
{
    int sum = 0;
    for (int i = 0; i < 3; ++i)
    {
        for (int x = 0; x < 3; ++x)
        {
            int num = matriz[i][x];
            sum += fabs(finalPosition[num][0] - i) + fabs(finalPosition[num][1] - x);
        }
    }

    cout << "\n"
         << sum;
    return sum;
}

bool algoritmoGuloso(vector<vector<int>> matriz)
{
    //cin.get();
    addVisitados(matriz);
    printMatriz(matriz);
    if (converteMatrizInt(matriz) == puzzleFinal)
    {
        cout << "\n RESOLVIDO";
        return true;
    }
    else
    {
        vector<listMatriz> filhosMatriz = retornaFilhos(matriz);
        int sizeFilhos = filhosMatriz.size(), i;

        int distanciaH[4] = {999999, 0, 0, 0}, indiceMenorH = -1;
        for (i = 0; i < sizeFilhos; i++)
        {
            distanciaH[i] = calculaN(filhosMatriz[i].puzzle);
            indiceMenorH = (distanciaH[i] <= distanciaH[indiceMenorH] && !matrizVisitada(converteMatrizInt(filhosMatriz[i].puzzle))) ? i : indiceMenorH;
        }

        cout << "min: " << distanciaH[indiceMenorH];

        if (indiceMenorH != -1)
        {
            //if()
            return algoritmoGuloso(filhosMatriz[indiceMenorH].puzzle);
        }

        return false;
    }
}

/** Retorna true caso a matriz esteja no vetor de visitados **/
bool matrizVisitada(int matrizInt)
{
    int i, retorno = false;
    for (i = 0; i < puzzleslVisitadas.size(); i++)
    {
        if (puzzleslVisitadas[i] == matrizInt)
        {
            retorno = true;
            break;
        }
    }
    return retorno;
}

/** Realiza o processamento, chamadas recursivas **/
bool buscaEmProfundidade(vector<vector<int>> matriz)
{
    addVisitados(matriz);
    printMatriz(matriz);
    if (converteMatrizInt(matriz) == puzzleFinal)
    {
        cout << "\n RESOLVIDO";
        return true;
    }
    else
    {
        vector<listMatriz> filhosMatriz = retornaFilhos(matriz);
        int sizeFilhos = filhosMatriz.size(), i;
        for (i = 0; i < sizeFilhos; i++)
        {
            if (!matrizVisitada(converteMatrizInt(filhosMatriz[i].puzzle)))
                if (buscaEmProfundidade(filhosMatriz[i].puzzle))
                    return true;
        }
        return false;
    }
}

/** Retorna os filhos possíveis para cada casa onde o zero esteja **/
vector<listMatriz> retornaFilhos(vector<vector<int>> matriz)
{
    vector<listMatriz> filhosMatriz;
    int aux = 0;
    vector<vector<int>> auxMatriz;
    /** auxMatriz é usada para alterações de configuração da matriz original **/
    /** Primeira linha de cada if **/
    auxMatriz = matriz;

    /** Ordem Flávio Excel **/
    if (matriz[0][0] == 0)
    {

        auxMatriz[0][0] = auxMatriz[1][0];
        auxMatriz[1][0] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[0][0] = auxMatriz[0][1];
        auxMatriz[0][1] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else if (matriz[0][1] == 0)
    {

        auxMatriz[0][1] = auxMatriz[0][0];
        auxMatriz[0][0] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[0][1] = auxMatriz[1][1];
        auxMatriz[1][1] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[0][1] = auxMatriz[0][2];
        auxMatriz[0][2] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else if (matriz[0][2] == 0)
    {
        auxMatriz[0][2] = auxMatriz[1][2];
        auxMatriz[1][2] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[0][2] = auxMatriz[0][1];
        auxMatriz[0][1] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else if (matriz[1][0] == 0)
    {
        auxMatriz[1][0] = auxMatriz[0][0];
        auxMatriz[0][0] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[1][0] = auxMatriz[2][0];
        auxMatriz[2][0] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[1][0] = auxMatriz[1][1];
        auxMatriz[1][1] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else if (matriz[1][1] == 0)
    {

        auxMatriz[1][1] = auxMatriz[2][1];
        auxMatriz[2][1] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[1][1] = auxMatriz[1][2];
        auxMatriz[1][2] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[1][1] = auxMatriz[1][0];
        auxMatriz[1][0] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[1][1] = auxMatriz[0][1];
        auxMatriz[0][1] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else if (matriz[1][2] == 0)
    {

        auxMatriz[1][2] = auxMatriz[2][2];
        auxMatriz[2][2] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[1][2] = auxMatriz[0][2];
        auxMatriz[0][2] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[1][2] = auxMatriz[1][1];
        auxMatriz[1][1] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else if (matriz[2][0] == 0)
    {
        auxMatriz[2][0] = auxMatriz[1][0];
        auxMatriz[1][0] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[2][0] = auxMatriz[2][1];
        auxMatriz[2][1] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else if (matriz[2][1] == 0)
    {

        auxMatriz[2][1] = auxMatriz[2][0];
        auxMatriz[2][0] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[2][1] = auxMatriz[2][2];
        auxMatriz[2][2] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[2][1] = auxMatriz[1][1];
        auxMatriz[1][1] = 0;
        filhosMatriz.push_back({auxMatriz});
    }
    else
    {
        //[2][2]

        auxMatriz[2][2] = auxMatriz[2][1];
        auxMatriz[2][1] = 0;
        filhosMatriz.push_back({auxMatriz});

        auxMatriz = matriz;
        auxMatriz[2][2] = auxMatriz[1][2];
        auxMatriz[1][2] = 0;
        filhosMatriz.push_back({auxMatriz});
    }

    return filhosMatriz;
}

/** Adiciona ao vetor de visistados, convertido para inteiro **/
void addVisitados(vector<vector<int>> matriz)
{
    puzzleslVisitadas.push_back(converteMatrizInt(matriz));
}

/** Converte uma matriz para um inteiro. Obs: 0 no início é removido **/
int converteMatrizInt(vector<vector<int>> matriz)
{
    stringstream saida;
    int i, j, retorno;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            saida << matriz[i][j];

    saida >> retorno;
    return retorno;
}

/** Printa matriz na tela **/
void printMatriz(vector<vector<int>> matriz)
{
    int i, j;
    cout << "\n.___________.\n";
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            cout << "| " << matriz[i][j] << " ";
        }
        cout << "|\n";
    }
    cout << "*-----------*";
    cout << "\n";
    cout << "\n Números de matrizes :" << puzzleslVisitadas.size() << "\n";
}
