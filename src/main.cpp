// Includes da STL
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <unistd.h>

// Bibliotecas
#define HAS_CODECVT
#include "archive.h"
#include "rapidcsv.h"

// Nossos headers
#include "BinarySearchTree.cpp"
#include "DataFileHandler.hpp"
#include "HashMap.hpp"
#include "IndexFileHandler.hpp"
#include "Reader.hpp"
#include "Registry.hpp"

using namespace std;

void initializeTree();
void initializeHash();

int readValidCode();
int readValidInnerMenuAnswer();
string readValidDate();

void menu();
void innerMenu();

void searchByCode();
void searchByCity();
void searchByDate();
void searchByVaccine();
void clearScreen();

void proveHypotheses();
void hypothesisOne();
void hypothesisTwo();
void hypothesisThree();

void searchByCode(vector<string> row);

DataFile *dataFile = new DataFile();
CodeIndex *codeIndex = new CodeIndex(*dataFile);
VaccineIndex *vaccineIndex = new VaccineIndex(*dataFile);
BinarySearchTree *cityIndex = new BinarySearchTree();
HashMap *dateIndex = new HashMap(*codeIndex);

int main() {

    initializeTree();
    initializeHash();

    int answer;

    cout << "-----------------------------------------------------------------" << endl;
    cout << "Ferramenta de pesquisa sobre a vacinação contra o COVID-19 no RS." << endl;
    
    do 
    {
        menu();

        while ((cout << "Escolha uma opção:" << endl ) 
                && !(cin >> answer) || answer < 0 || answer > 5) 
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
            case 5:
                proveHypotheses();
                break;

        }
        
    } while (answer);
}

void initializeTree()
{
    /**
     * @brief Inicializa a árvore binária de pesquisa, varrendo
     * todo o arquivo de dados e agregando registros com base na
     * cidade de aplicação da vacina.
     */

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

void initializeHash()
{
    /**
     * @brief Inicializa o hash map, varrendo o arquivo de dados e agregando
     * todos os registros com base na data de aplicação da vacina.
     */
    cout << "Gerando o mapa de hash..." << endl;

    Registry temp;
    Archive <ifstream> *archiveIn = dataFile->archiveIn;
    int position;

    for (int code = 0; code < dataFile->size(); code++){
        position = dataFile->iFile.tellg();
        *archiveIn >> temp;
        dateIndex->insertNewValue(temp.application_date, temp.patient_code);
    }

    dataFile->resetInputStream();
}

int readValidCode()
{
    /**
     * @brief Lê um código válido para ser pesquisado no arquivo de índices.
     * Por válido, entende-se um código numérico e maior que zero.
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
    return code;
}

int readValidInnerMenuAnswer()
{
    /**
     * @brief Lê uma resposta válida para o menu interno, que apresenta
     * duas opções de pesquisa secundária sobre o resultado de uma pesquisa.
     */
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

string readValidDate()
{
    /**
     * @brief Lê uma data no formato YYYY-MM-DD válida para pesquisa no
     * hash map.
     */
    static const regex dateFormat(R"([12]\d{3}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01]))"); 
    string date;

    while ((cout << "Insira uma data, no formato YYYY-MM-DD." << endl ) 
            && !(cin >> date) || !regex_match(date, dateFormat)) 
    {
        cout << "Data Inválida!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << endl;
    return date;
}

void menu()
{
    cout << "Escolha uma opção a seguir:" << endl;
    cout << "1) Procurar um registro pelo seu código." << endl;
    cout << "2) Procurar todos os registros de uma cidade." << endl;
    cout << "3) Procurar todos os registros de uma data." << endl;
    cout << "4) Procurar todos os pacientes vacinados com uma vacina." << endl;
    cout << "5) Provar hipóteses." << endl;
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
    /**
     * @brief Função que lê uma cidade e procura todos os registros que
     * tiveram a aplicação da vacina nessa cidade.
     * Permite fazer uma pesquisa de código secundária sobre esse resultado.
     */
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
    /**
     * @brief Função que lê uma data e permite procurar todos os registros que
     * tiveram a aplicação da vacina nessa data.
     */
    dateIndex->printAllValuesFromKey(readValidDate());
    sleep(3);
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
    int vaccine_count = vaccineIndex->firstLayerSize();

    // Pegar o nome de todas as vacinas
    for (int count = 0; count < vaccineIndex->firstLayerSize(); count++)
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
            && !(cin >> code) || code < 0 || code > vaccineIndex->firstLayerSize()) 
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

void proveHypotheses()
{
    hypothesisOne();
    hypothesisTwo();
    hypothesisThree();
}

void hypothesisOne()
{
    /**
     * @brief Prova se a Coronavac é a vacina mais utilizada na vacinação
     * por ter sua produção no Brasil.
     */

    Reader *reader = new Reader(vaccineIndex->fileName);
    vector<string> row;
    int vaccine_count = vaccineIndex->firstLayerSize();

    cout << "Hipótese 1: Coronavac é a vacina mais utilizada na vacinação por ter sua produção no Brasil" << endl;

    for (int i = 0; i < vaccine_count; i++)
    {
        row = reader->getRow(i);
        cout << "Vacina: " << row[1] << " Número de vacinados: " << row[3] << endl;
    }

    cout << "De fato, a coronavac é a vacina mais utilizada no Brasil." << endl << endl;
    sleep(3);
}

void hypothesisTwo()
{
    /**
     * @brief Prova se a quantidade de pessoas vacinadas é proporcional à
     * quantidade de habitantes por município.
     */

    cout << "Hipótese 2: A quantidade de pessoas vacinadas é proporcional a quantidade de habitantes por município" << endl;
    cout << endl;

    cityIndex->inOrderPrint();

    const char *output =
        "Como mostram os resultados, Porto Alegre e Caxias do Sul contêm"
        " uma grande parte dos vacinados, justamente por serem cidades"
        " com uma população elevada";

    cout << output << endl; 
    sleep(3);
}

void hypothesisThree()
{
    /**
     * @brief Prova se há maior quantidade de vacinados na categoria de
     * profissionais da saúde do que nas outras categorias.
     */

    cout << "Hipótese 3: Há maior quantidade de vacinados na categoria de ‘Profissionais da Saúde" << endl;
    cout << endl;

    map<string, int> categories;
    Registry temp;
    string category;
    Archive<ifstream> *archiveIn = dataFile->archiveIn;

    for (int i = 0; i < dataFile->size(); i++) {
        *archiveIn >> temp;
        category = temp.category_name;

        if (categories.find(category) != categories.end())
        {
            categories.at(category)++;
        } else {
            categories.insert(make_pair(category, 1));   
        }
        
    }

    for (auto & category : categories)
    {
        cout << "Categoria: " << category.first << " Número de vacinados: " << category.second << endl;
    }

    cout << "Evidentemente, a quantidade de vacinados pela faixa etária é mais do que 3 vezes maior." << endl;
    sleep(3);
}