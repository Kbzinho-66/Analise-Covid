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

int main() {

<<<<<<< HEAD
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
=======
    DataFile *dataFile = new DataFile();
>>>>>>> 91ee7266bf5313bb62867ce4d732e789ff07f170
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
    //TODO
}