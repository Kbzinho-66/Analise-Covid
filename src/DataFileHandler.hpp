#include "archive.h"
#include <iostream>
#include <fstream>
#include "Reader.hpp"
#include "Registry.hpp"

#define ARQUIVO_BINARIO "RS_Mini.bin"

using namespace std;

class DataFile
{
public:
    void printBinaryDataFile();
    Registry *getRegistryByAddress(const int address);
    int size();


    DataFile(void);
    ~DataFile(void);

private:
    ifstream iFile;
    ofstream oFile;

    void generateBinaryDataFile();
};

DataFile::DataFile()
{
    /**
     * @brief Construtor da classe DataFile.
     * Tenta abrir o arquivo de dados, se conseguir, cria tanto o stream de 
     * input quanto o de output. Se não conseguir, primeiro gera o arquivo,
     * depois seta os streams.
     * 
     * @bug Só funciona quando o arquivo de dados não foi gerado ainda,
     * mas não descobri o porquê.
     */

    iFile.open(ARQUIVO_BINARIO, ios::in | ios::binary);

    if (!iFile) 
    {
        generateBinaryDataFile();
        iFile.open(ARQUIVO_BINARIO, ios::in  | ios::binary);
    }

}

DataFile::~DataFile()
{
    iFile.close();
    oFile.close();
}

int DataFile::size() 
{
    return (ARQUIVO_BINARIO == "RS_Mini.bin") ? 50 : 100000;
}

void DataFile::generateBinaryDataFile() {

    /**
     * @brief Função para gerar o arquivo de dados binário
     * a partir de um arquivo .csv .
     * 
     * Essa função é chamada somente quando não foi possível
     * abrir um arquivo de dados binário.
     */

    oFile.open(ARQUIVO_BINARIO, ios::out | ios::binary);

    if(!oFile) {
        cout << "Não foi possível criar o arquivo de dados." << endl;
        exit(0);
    }

    Reader *reader = new Reader();

    Archive<ofstream> archiveOut(oFile);

    for(int patient = 0; patient < size(); patient++) {

        vector<string> row = reader->getRow(patient);

        Registry *entry = new Registry(row);

        archiveOut << entry;
    }   

    oFile.seekp(0, oFile.beg);
}

void DataFile::printBinaryDataFile() {

    /**
     * @brief A princípio só mostra todos os registros
     * do arquivo pra ver se tá tudo funcionando.
     */

    Registry temp;

    Archive<ifstream> archiveIn(this->iFile);

    for (int line = 1; line <= size(); line++) 
    {
        cout << "Posição no arquivo:" << this->iFile.tellg() << endl;
        archiveIn >> temp;
        temp.printRegistryInfo();
    }

    this->iFile.seekg(0, this->iFile.beg);
}

Registry *DataFile::getRegistryByAddress(const int address)
{
    /**
     * @brief Método chamado por qualquer função de pesquisa para
     * "converter" um endereço para um Registro.
     * 
     * @param address O endereço específico do Registro no arquivo.
     * 
     * @return O objeto Registro pesquisado.
     */

    Registry *result;
    Archive<ifstream> archiveIn(iFile);

    iFile.seekg(address, iFile.beg);
    archiveIn >> result;
    iFile.seekg(0, iFile.beg);

    return result;
}