// Includes da STL
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

// Bibliotecas
#define HAS_CODECVT
#include "archive.h"
#include "rapidcsv.h"

// Nossos headers
#include "DataFileHandler.hpp"
#include "IndexFileHandler.hpp"
#include "Reader.hpp"
#include "Registry.hpp"

using namespace std;

void menu();
void searchByCode();
void searchByCity();
void searchByDate();
void searchByVaccine();
void clearScreen();

DataFile *dataFile = new DataFile();
CodeIndex *codeIndex = new CodeIndex(*dataFile);
VaccineIndex *vaccineIndex = new VaccineIndex(*dataFile);

int main() {

    int answer;

    cout << "-----------------------------------------------------------------" << endl;
    cout << "Ferramenta de pesquisa sobre a vacinação contra o COVID-19 no RS." << endl;
    
    do 
    {
        menu();

        while ((cout << "Escolha uma opção:" << endl ) 
                && !(cin >> answer) || answer < 0 || answer > 4) 
        {
            cout << "Opção Inválida!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch(answer) 
        {
            case 1: 
                searchByCode();
                break;

            case 2:
                searchByCity();
                break;

            case 3:
                searchByDate();
                break;

            case 4:
                searchByVaccine();
                break;
        }
        
    } while (answer);
}

void menu()
{
    cout << "Escolha uma opção a seguir:" << endl;
    cout << "1) Procurar um registro pelo seu código." << endl;
    cout << "2) Procurar todos os registros de uma cidade." << endl;
    cout << "3) Procurar todos os registros de uma data." << endl;
    cout << "4) Procurar todos os pacientes vacinados com uma vacina." << endl;
    cout << "0) Sair." << endl;
}

void searchByCode()
{
    /**
     * @brief Função que pega um código válido e chama o método pra encontrar
     * o Registro correspondente.
     */
    int code;
    while ((cout << "Insira o código a ser pesquisado:" << endl ) 
            && !(cin >> code) || code < 0) 
    {
        cout << "Código Inválido. Somente inteiros positivos são permitidos" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << endl;
    codeIndex->searchRegistryByCode(code);
    sleep(3);
}

void searchByCity()
{
    //TODO
}

void searchByDate()
{
    //TODO
}

void searchByVaccine()
{
    /**
     * @brief Função para procurar todos os Registros de uma vacina.
     * Permite procurar nesses Registros por outro parâmetro.
     */

    Reader *reader = new Reader(vaccineIndex->fileName);
    vector<string> row;
    vector<pair<int, string>> vaccines;

    // Pegar o nome de todas as vacinas
    for (int count = 0; count < vaccineIndex->firstLayerSize; count++)
    {
        row = reader->getRow(count);
        vaccines.push_back(make_pair(count + 1, row[1]));
    }

    // Fazer um menuzinho
    cout << "Vacinas encontradas:" << endl;
    for (auto & vaccine : vaccines)
    {
        cout << vaccine.first << ") " << vaccine.second << endl;
    }
    
    // Laço pra ler um código válido
    int code;
    while ((cout << "Selecione o código desejado:" << endl) 
            && !(cin >> code) || code < 0 || code > vaccineIndex->firstLayerSize) 
    {
        cout << "Código Inválido." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Encontrar o arquivo certo e se for necessário, abrir ele
    for (auto & vaccine : vaccines)
    {
        if (code == vaccine.first) {
            row = reader->getRow(code-1);
            cout << "Foram encontrados " << row[3] << " pacientes vacinados com essa vacina." << endl;
            sleep(2);
            //TODO (Fazer uma segunda consulta nesses dados)
        }
    }
}