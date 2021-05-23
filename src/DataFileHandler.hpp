#pragma once

// Includes da STL
#include <iostream>
#include <fstream>

// Bibliotecas
#include "archive.h"

// Nossos headers
#include "Reader.hpp"
#include "Registry.hpp"

// Nomes dos arquivos - Descomentar aqui qual arquivo vai ser usado
// e não esquecer de deletar os arquivos já gerados.
#define ARQUIVO_TEXTO "RS_Mini.csv"
// #define ARQUIVO_TEXTO "RS_Pequeno.csv"

#define ARQUIVO_BINARIO "RS.bin"

using namespace std;

class DataFile
{
public:
    string fileName;
    ifstream iFile;
    Archive<ifstream> *archiveIn;

    DataFile(void);
    ~DataFile(void);
    
    void printBinaryDataFile();
    Registry getRegistryByAddress(const int address);
    constexpr int size();


private:

    void generateBinaryDataFile();
};

DataFile::DataFile()
{
    /**
     * @brief Construtor da classe DataFile.
     * Tenta abrir o arquivo de dados, se conseguir, abre um stream de input.
     * Se não conseguir, primeiro gera o arquivo, depois abre o stream.
     */

    fileName = ARQUIVO_TEXTO;
    iFile.open(ARQUIVO_BINARIO, ios::in | ios::binary);

    if (!iFile) 
    {
        generateBinaryDataFile();
        iFile.open(ARQUIVO_BINARIO, ios::in  | ios::binary);
    }

    archiveIn = new Archive(iFile);
    // printBinaryDataFile();
}

DataFile::~DataFile()
{
    iFile.close();
}

constexpr int DataFile::size() 
{
    /**
     * @brief Retorna a quantidade de entradas no arquivo de acordo com
     * o nome do arquivo texto usado.
     */
    return (ARQUIVO_TEXTO == "RS_Mini.csv") ? 50 : 100000;
}

void DataFile::generateBinaryDataFile() {

    /**
     * @brief Função para gerar o arquivo de dados binário
     * a partir de um arquivo .csv .
     * 
     * Essa função é chamada somente quando não foi possível
     * abrir um arquivo de dados binário.
     */

    ofstream oFile(ARQUIVO_BINARIO, ios::out | ios::binary);

    if(!oFile) {
        cout << "Não foi possível criar o arquivo de dados." << endl;
        exit(0);
    }

    cout << "Gerando o arquivo de dados..." << endl;

    Reader *reader = new Reader(ARQUIVO_TEXTO);
    Archive<ofstream> archiveOut(oFile);

    for(int index = 0; index < size(); index++) 
    {
        vector<string> row = reader->getRow(index);
        Registry *entry = new Registry(row);
        archiveOut << entry;
    }   

    oFile.close();
}

void DataFile::printBinaryDataFile() {

    /**
     * @brief Mostra o primeiro e o último Registros gravados no arquivo.
     * Método usado somente para fins de teste.
     */

    Registry temp;

    for (int code = 1; code <= size(); code++) 
    {
        *archiveIn >> temp;
        
        if (code == 1 || code == size()) 
        {
            temp.printRegistry();
        }
        
    }

    iFile.seekg(0, iFile.beg);
}

Registry DataFile::getRegistryByAddress(const int address) 
{
    /**
     * @brief Método chamado por qualquer função de pesquisa para
     * "converter" um endereço para um Registro.
     * 
     * @param address O endereço específico do Registro no arquivo.
     * 
     * @return O objeto Registro pesquisado.
     */

    Registry result;

    iFile.seekg(address, iFile.beg);
    *archiveIn >> result;
    iFile.seekg(0, iFile.beg);

    return result;
}