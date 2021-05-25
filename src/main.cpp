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
#include "BinarySearchTree.cpp"
#include "DataFileHandler.hpp"
#include "IndexFileHandler.hpp"
#include "Reader.hpp"
#include "Registry.hpp"

using namespace std;

void initializeTree();

int readValidCode();
void menu();

void searchByCode();
void searchByCity();
void searchByDate();
void searchByVaccine();
void clearScreen();

void searchByCode(vector<string> row);

DataFile *dataFile = new DataFile();
CodeIndex *codeIndex = new CodeIndex(*dataFile);
VaccineIndex *vaccineIndex = new VaccineIndex(*dataFile);
BinarySearchTree *cityIndex = new BinarySearchTree();

int main() {

    initializeTree();

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

void initializeTree()
{
    cout << "Gerando a árvore binária de pesquisa..." << endl;

    Registry temp;
    Archive<ifstream> *archiveIn = dataFile->archiveIn;

    for (int code = 0; code < dataFile->size(); code++)
    {
        *archiveIn >> temp;
        cityIndex->insertIndex(temp.application_city, temp.patient_code);
    }

    dataFile->resetInputStream();
}

int readValidCode()
{
    int code;
    while ((cout << "Insira o código a ser pesquisado:" << endl ) 
            && !(cin >> code) || code < 0) 
    {
        cout << "Código Inválido. Somente inteiros positivos são permitidos" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << endl;
    return code;
}

int readValidInnerMenuAnswer()
{
    int answer;
    while ((cout << "Escolha uma opção:" << endl ) 
            && !(cin >> answer) || answer < 0 || answer > 2) 
    {
        cout << "Opção Inválida!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << endl;
    return answer;
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

void innerMenu()
{
    cout << endl;
    cout << "Escolha uma opção:" << endl;
    cout << "1) Procurar um código nesses registros." << endl;
    cout << "2) Mostrar alguns dos registros." << endl;
    cout << "0) Voltar para o menu principal." << endl;
}

void searchByCode()
{
    /**
     * @brief Função que pega um código válido e chama o método pra encontrar
     * o Registro correspondente.
     */
    
    codeIndex->searchRegistryByCode(readValidCode());
    sleep(3);
}

void searchByCity()
{
    string city_name;
    int answer, code;
    bool found = false;

    cout << "Insira o nome da cidade (Sem acentos): " << endl;
    getchar();
    getline(cin, city_name);

    transform(city_name.begin(), city_name.end(), city_name.begin(),
        [](unsigned char c){ return toupper(c);});

    Node *result = cityIndex->searchCity(city_name);

    cout << "Foram encontrados " << result->indexes.size() << " registros nessa cidade." << endl;

    do
    {
        innerMenu();
        answer = readValidInnerMenuAnswer();        

        switch (answer)
        {
            case 1:
            {
                code = readValidCode();
                found = false;
                for (int & patient_code : result->indexes)
                {
                    if (patient_code == code)
                    {
                        codeIndex->searchRegistryByCode(patient_code);  
                        sleep(3);
                        found = true;
                        break;
                    } 
                }

                if (!found)
                    cout << "Código não encontrado." << endl;
                
                break;
            }

            case 2:
                for (int index = 0; index < result->indexes.size(); index++)
                {
                    if (index < 5 || index > result->indexes.size() - 6)
                        codeIndex->searchRegistryByCode(result->indexes[index]);
                }
                sleep(3);
                break;
        }

    } while (answer);

}

void searchByDate()
{
    //TODO
}

void searchByVaccine()
{
    /**
     * @brief Função para procurar todos os Registros de uma vacina.
     * Permite procurar nesses Registros por um código específico.
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
        if (code == vaccine.first) 
        {
            int answer;

            row = reader->getRow(code-1);
            cout << "Foram encontrados " << row[3] << " pacientes vacinados com essa vacina." << endl << endl;
            sleep(2);
            
            do
            {
                innerMenu();
                answer = readValidInnerMenuAnswer();

                switch (answer)
                {
                    case 1:
                        searchByCode(row);
                        break;

                    case 2:
                        vaccineIndex->printVaccineFile(row[2], stoi(row[3]));
                        break;
                }

            } while (answer);
            
        }
    }
}

void searchByCode(vector<string> row)
{
    /**
     * @brief Função que pega um código válido e chama o método pra encontrar
     * o Registro correspondente em um arquivo específico.
     * 
     * @param row Um vetor de strings que contém:
     * [0] = código da vacina
     * [1] = nome da vacina
     * [2] = nome do arquivo de índices
     * [3] = tamanho do arquivo de índices
     * 
     */
    string fileName = row[2];
    int size = stoi(row[3]);

    int code;
    while ((cout << "Insira o código a ser pesquisado:" << endl ) 
            && !(cin >> code) || code < 0) 
    {
        cout << "Código Inválido. Somente inteiros positivos são permitidos" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << endl;
    vaccineIndex->searchRegistryByCode(fileName, code, size);
    sleep(3);
}