    /**
 * @file IndexFileHandler.hpp
 * 
 * @brief Arquivo onde estão as classes e os métodos que lidarão com qualquer
 * operação envolvendo arquivos de índice.
 * 
 */

#pragma once

// Includes da STL
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <unistd.h>

// Bibliotecas
#define HAS_CODECVT
#include "archive.h"

// Nossos headers
#include "DataFileHandler.hpp"
#include "Registry.hpp"

// Nomes dos arquivos
#define ARQUIVO_INDICES_CODIGO "Indices_Codigo_RS.bin"
#define ARQUIVO_INDICES_VACINA "Indices_Vacina_RS.txt"

// Flag pra forçar a geração de todos os arquivos
#define FLAG_TESTE false

/////////////////////////////////////////////////////////////////////
/////          ÍNDICES ORDENADOS PELO PACIENTE_CODIGO           /////
/////////////////////////////////////////////////////////////////////

#define TAMANHO_INDEX (sizeof(int) * 2)

std::string indexFile;

class CodeIndex
{
    /**
     * @brief Classe para representar o arquivo de índices específico
     * para a coluna paciente_codigo.
     */
    public:
        std::string indexFileName;

        CodeIndex(DataFile &dataFile_);
        ~CodeIndex(void);

        int size(void);
        void searchRegistryByCode(int patient_code);

    private:
        std::ifstream iIndexFile;
        DataFile &dataFile;

        void printCodeIndexFile(void);
        void generateCodeIndexFile(void);
        int binarySearch(Archive<ifstream> archiveIn, int code, int begin, int end);
};

CodeIndex::CodeIndex(DataFile &dataFile_) : dataFile(dataFile_)
{
    /**
     * @brief Construtor para a classe CodeIndex.
     * Tenta abrir um arquivo de índices. Se conseguir, abre um stream de input.
     * Se não conseguir, gera um arquivo de índices e então abre o stream.
     * 
     * @param dataFile O arquivo de dados atualmente aberto.
     */
    
    if (FLAG_TESTE)
    {
        generateCodeIndexFile();
        iIndexFile.open(ARQUIVO_INDICES_CODIGO, ios::in | ios::binary);
    }
    else
    {
        int size;

        // Tenta abrir o arquivo de índices pra leitura
        iIndexFile.open(ARQUIVO_INDICES_CODIGO, ios::in | ios::binary);
        size = iIndexFile.seekg(0, iIndexFile.end).tellg();
        iIndexFile.seekg(0, iIndexFile.beg);

        if (!iIndexFile || size == 0)
        {
            // Se não conseguiu, gerar o arquivo e abrir pra leitura
            generateCodeIndexFile();
            iIndexFile.open(ARQUIVO_INDICES_CODIGO, ios::in | ios::binary);
        }
    }

    // printCodeIndexFile();
}

CodeIndex::~CodeIndex()
{
    iIndexFile.close();
}

int CodeIndex::size()
{   
    /**
     * @brief Método que calcula a quantidade de índices guardados em um arquivo.
     * 
     * @returns O número de índices.
     */
    iIndexFile.seekg(0, iIndexFile.end);
    int size = iIndexFile.tellg() / TAMANHO_INDEX;
    iIndexFile.seekg(0, iIndexFile.beg);
    return size;
}

void CodeIndex::generateCodeIndexFile() 
{
    /**
     * @brief Função que vai gerar o arquivo de índices
     * especificamente para a coluna paciente_codigo.
     */

    // Abrir o arquivo de índices para escrita
    ofstream oIndexFile(ARQUIVO_INDICES_CODIGO, ios::out | ios::binary);

    if (!oIndexFile) {
        cout << "Não foi possível criar o arquivo de índices." << endl;
        exit(0);
    }

    cout << "Gerando o arquivo de índices para código..." << endl;

    Archive<ifstream> *archiveIn = dataFile.archiveIn;
    Archive<ofstream> archiveOut(oIndexFile);
    Registry temp;
    int code, position;

    for (int index = 1; index <= dataFile.size(); index++) {
        /**
         * Varredura do arquivo de dados, agregando um patient_code a uma 
         * posição específica no arquivo de dados. Essa associação é 
         * então gravada no arquivo de índices como um par.
         */

        position = dataFile.iFile.tellg();
        *archiveIn >> temp;
        code = temp.patient_code;
        archiveOut << make_pair(code, position);
    }

    dataFile.resetInputStream();
    oIndexFile.close();
}

void CodeIndex::printCodeIndexFile() {

    /**
     * @brief Printa todo o arquivo de índices
     * pra ver se tá tudo funcionando.
     * Método usado somente para testes.
     */

    Archive<ifstream> archiveIn(iIndexFile);

    pair<int, int> index;

    for (int line = 0; line < dataFile.size(); line++) {

        cout << "Posição do Índice:" << iIndexFile.tellg();
        archiveIn >> index;
        cout << " Código: " << index.first << " Posição no arquivo de dados: " << index.second << endl;
    }

    iIndexFile.seekg(0, iIndexFile.beg);
}

void CodeIndex::searchRegistryByCode(int patient_code)
{
    /**
     * @brief Método para procurar o endereço de um registro no arquivo
     * de índices.
     * 
     * @param patient_code O código do paciente a ser retornado
     */

    Archive<ifstream> archiveInIndex(iIndexFile);
    int address = binarySearch(archiveInIndex, patient_code, 0, size());

    if (address != -1) dataFile.getRegistryByAddress(address).printRegistryInfo();
    else cout << "Código não encontrado." << endl << endl;
}

int CodeIndex::binarySearch(Archive<ifstream> archiveInIndex, int code, int begin, int end) 
{
    /**
     * @brief Função que faz pesquisa binária no arquivo de índices para
     * encontrar o paciente_codigo pesquisado.
     * 
     * @param archiveInIndex O objeto Archive responsável pela serialização
     * e desserialização dos índices.
     * @param code O paciente_codigo pesquisado
     * @param begin A posição inicial da parte do arquivo que será pesquisada
     * @param end A posição final da parte do arquivo que será pesquisada
     */
    pair<int,int> index;
    int middle = (begin + end) / 2;
    
    if (middle >= size()) return -1;
    iIndexFile.seekg(middle * TAMANHO_INDEX);
    archiveInIndex >> index;

    if (code == index.first) {
        return index.second;
    } else if (code < index.first && begin != end) {
        return binarySearch(archiveInIndex, code, begin, middle);
    } else if (begin != end) {
        return binarySearch(archiveInIndex, code, middle+1, end);
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////
/////          ÍNDICES ORDENADOS PELO NOME DA VACINA            /////
/////////////////////////////////////////////////////////////////////

class VaccineIndex
{
    /**
     * @brief Classe para representar o arquivo de índices específico
     * para a coluna vacina_nome.
     */
    public:
        std::string fileName;

        VaccineIndex(DataFile &dataFile_);
        ~VaccineIndex(void);

        int firstLayerSize(void);
        void searchRegistryByVaccine_Name(string vaccine_name);
        void searchRegistryByCode(string fileName, int patient_code, int size);
        void printVaccineFile(string fileName, int vaccine_count);

    private:
        std::ifstream iIndexFile;
        DataFile &dataFile;

        void printVaccineIndexFile(void);
        void generateVaccineIndexFile(void);
        vector<tuple<string, int, int>> generateFirstLayer(void);
        void generateSecondLayer(const vector<tuple<string, int, int>> &vaccines);
        int binarySearch(Archive<ifstream> archiveInIndex, int code, int begin, int end, int size) ;
        bool codeNotInVector(int code, vector<tuple<string, int, int>> &vaccines);
};

VaccineIndex::VaccineIndex(DataFile &dataFile_): dataFile(dataFile_)
{
    /**
     * @brief Construtor para a classe VaccineIndex.
     * Tenta abrir um arquivo de índices. Se conseguir, abre um stream de input.
     * Se não conseguir, gera um arquivo de índices e então abre o stream.
     * 
     * @param dataFile O arquivo de dados atualmente aberto.
     */

    fileName = ARQUIVO_INDICES_VACINA;
    // firstLayerSize = 0;

    if (FLAG_TESTE)
    {
        generateVaccineIndexFile();
        iIndexFile.open(ARQUIVO_INDICES_VACINA, ios::in);
    }
    else
    {
        int size;
        // Tenta abrir o arquivo de índices pra leitura
        iIndexFile.open(ARQUIVO_INDICES_VACINA, ios::in);
        size = iIndexFile.seekg(0, iIndexFile.end).tellg();
        iIndexFile.seekg(0, iIndexFile.beg);

        if (!iIndexFile || size == 0)
        {
            // Se não conseguiu, gerar o arquivo e abrir pra leitura
            generateVaccineIndexFile();
            iIndexFile.open(ARQUIVO_INDICES_VACINA, ios::in);
        }
    }
    
    // printCodeIndexFile();
}

VaccineIndex::~VaccineIndex()
{
    iIndexFile.close();
}

int VaccineIndex::firstLayerSize()
{
    /**
     * @brief Maneira nada elegante de saber o tamanho da primeira camada do
     * arquivo de vacinas.  
     */
    return (dataFile.fileName == "RS_Mini.csv") ? 2 : 3;
}

void VaccineIndex::generateVaccineIndexFile() 
{
    /**
     * @details Função que vai gerar duas camadas de arquivos de índices
     * para a coluna vacina_nome.
     * 
     * A primeira é um arquivo com o nome de cada vacina e o nome dos arquivos
     * que compõem a segunda camada, que contêm o código e endereço no arquivo
     * de dados de cada paciente vacinado com aquela vacina.
     */

    // Gerar um vetor que contém <nome, código, frequência> de cada vacina
    vector<tuple<string, int, int>> vaccines = generateFirstLayer();

    generateSecondLayer(vaccines);
    
}

vector<tuple<string, int, int>> VaccineIndex::generateFirstLayer()
{
    /**
     * @brief Gera a primeira camada de índices. Essa camada contém o nome e o
     * código de cada vacina presente no arquivo de dados, além do nome do
     * nome e tamanho do arquivo de índices referente a cada vacina.
     * 
     * @return Um vetor agregando <nome, código, frequência> de cada vacina.
     */

    // Abrir o arquivo da primeira camada para escrita em texto
    ofstream oFirstIndexFile(ARQUIVO_INDICES_VACINA, ios::out);

    if (!oFirstIndexFile) {
        cout << "Não foi possível criar a primeira camada de índices." << endl;
        exit(0);
    }

    cout << "Gerando a primeira camada dos índices de vacina..." << endl;

    // Gerar um vetor com o nome de todas vacinas no arquivo de dados e quantas
    // vezes cada uma aparece.
    vector<tuple<string, int, int>> vaccines;
    Archive<ifstream> *archiveIn = dataFile.archiveIn;
    Registry temp;
    string name;
    int code;

    for (int index = 1; index <= dataFile.size(); index++) {
        /**
         * Varredura do arquivo de dados, coletando os diferentes nomes de
         * vacinas utilizados.
         */

        *archiveIn >> temp;
        name = temp.vaccine_name;
        code = temp.vaccine_code;
        if (codeNotInVector(code, vaccines)) 
        {
            vaccines.push_back(make_tuple(name, code, 1));
            // firstLayerSize++;
        }

    }

    dataFile.resetInputStream();

    // Escrever tudo no .csv dos índices
    oFirstIndexFile << "Vacina_Codigo,Vacina_Nome,Arquivo_Nome,Tamanho_Arquivo" << endl;
    for (auto & vaccine : vaccines) {
        oFirstIndexFile << get<1>(vaccine)                  //Código da vacina
        << "," << get<0>(vaccine)                           //Nome da vacina
        << "," << "Vacina_" << get<1>(vaccine) << ".bin"    //Nome do arquivo
        << "," << get<2>(vaccine) << endl;                  //Tamanho do arquivo             
    }

    oFirstIndexFile.close();

    return vaccines;
}

void VaccineIndex::generateSecondLayer(const vector<tuple<string, int, int>> &vaccines)
{
    /**
     * @brief Gera a segunda camada de índices. Essa camada contém somente uma
     * associação entre um paciente_codigo e o seu endereço no arquivo de dados.
     * 
     * @param vaccines O vetor gerado em generateFirstLayer() que contém o
     * <nome, código, frequência> de cada vacina.
     */

    cout << "Gerando a segunda camada dos índices de vacina..." << endl;

    string fileName;
    ofstream oVaccineFile;
    Archive<ifstream> *archiveIn = dataFile.archiveIn;
    Archive<ofstream> *archiveOut;
    Registry temp;
    int patient_code, vaccine_code, position, vaccine_count, count;
    string name;

    for (auto & vaccine : vaccines)
    {
        vaccine_code = get<1>(vaccine);
        vaccine_count = get<2>(vaccine);

        //Abrir um arquivo de índices binário pra essa vacina
        fileName = "Vacina_" + to_string(vaccine_code) + ".bin";
        oVaccineFile.open(fileName, ios::out | ios::binary);
        archiveOut = new Archive(oVaccineFile);

        //Escrever todos os registros com essa vacina
        count = 0;
        while (count < vaccine_count) 
        {
            position = dataFile.iFile.tellg();
            *archiveIn >> temp;
            patient_code = temp.patient_code;


            if (temp.vaccine_code == vaccine_code) 
            {
                *archiveOut << make_pair(patient_code, position);
                count++;
            }
        }

        dataFile.resetInputStream();
        oVaccineFile.close();

        // printVaccineFile(fileName, vaccine_count);
    }
}

void VaccineIndex::printVaccineFile(string fileName, int vaccine_count)
{
    /**
     * @brief Função para mostrar os 5 primeiros e últimos registros de um arquivo
     */

    ifstream iFile;
    iFile.open(fileName, ios::in);
    Archive<ifstream> archiveIn(iFile);
    pair<int,int> index;

    cout << "=========================================================" << endl;

    for (int count = 0; count < vaccine_count; count++)
    {
        archiveIn >> index;
        if (count < 5 || count > vaccine_count - 6)
        {
            dataFile.getRegistryByAddress(index.second).printRegistryInfo();
        }
    }

    iFile.close();
    sleep(3);
}

bool VaccineIndex::codeNotInVector(int code, vector<tuple<string, int, int>> &vaccines)
{
    /**
     * @brief Verifica a presença de um vacina_codigo no vetor de vacinas.
     * Se o código está presente, incrementa a frequência da vacina para indicar
     * a busca repetida.
     * 
     * @returns true: Quando o código não foi encontrado
     * @returns false: Quando o código foi encontrado
     */
    for (auto & vaccine : vaccines)
    {
        if (get<1>(vaccine) == code)
        {
            get<2>(vaccine)++;
            return false;
        }
    }

    return true;
}

void VaccineIndex::searchRegistryByCode(string fileName, int patient_code, int size)
{
    /**
     * @brief Método para procurar o endereço de um registro em um arquivo
     * de índices específico.
     * 
     * @param fileName O arquivo específico de uma vacina
     * 
     * @param patient_code O código do paciente a ser retornado
     */

    ifstream iFile(fileName, ios::in | ios::binary);

    Archive<ifstream> archiveInIndex(iFile);
    int address = binarySearch(archiveInIndex, patient_code, 0, size, size);

    if (address != -1) dataFile.getRegistryByAddress(address).printRegistryInfo();
    else cout << "Código não encontrado." << endl << endl;
}

int VaccineIndex::binarySearch(Archive<ifstream> archiveInIndex, int code, int begin, int end, int size) 
{
    /**
     * @brief Função que faz pesquisa binária no arquivo de índices para
     * encontrar o paciente_codigo pesquisado.
     * 
     * @param archiveInIndex O objeto Archive responsável pela serialização
     * e desserialização dos índices.
     * @param code O paciente_codigo pesquisado
     * @param begin A posição inicial da parte do arquivo que será pesquisada
     * @param end A posição final da parte do arquivo que será pesquisada
     * @param size O tamanho do arquivo de índices
     */

    pair<int,int> index;
    int middle = (begin + end) / 2;
    
    if (middle >= size) return -1;
    iIndexFile.seekg(middle * TAMANHO_INDEX);
    archiveInIndex >> index;

    if (code == index.first) {
        return index.second;
    } else if (code < index.first && begin != end) {
        return binarySearch(archiveInIndex, code, begin, middle, size);
    } else if (begin != end) {
        return binarySearch(archiveInIndex, code, middle+1, end, size);
    }

    return -1;
}