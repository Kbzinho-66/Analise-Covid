#pragma once

// Includes da STL
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Bibliotecas
#define HAS_CODECVT
#include "archive.h"

// Nossos headers
#include "DataFileHandler.hpp"
#include "Registry.hpp"

// Nomes dos arquivos
#define ARQUIVO_INDICES "Indices_Codigo_RS.bin"

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

        CodeIndex(DataFile &dataFile_, std::string indexFileName);
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

CodeIndex::CodeIndex(DataFile &dataFile_, std::string indexFileStr = ARQUIVO_INDICES): dataFile(dataFile_)
{
    /**
     * @brief Construtor para a classe CodeIndex.
     * Tenta abrir um arquivo de índices. Se conseguir, abre um stream de input.
     * Se não conseguir, gera um arquivo de índices e então abre o stream.
     * 
     * @param dataFile O arquivo de dados atualmente aberto.
     */
    
    this->indexFileName = indexFileStr;

    // Tenta abrir o arquivo de índices pra leitura
    iIndexFile.open(this->indexFileName, ios::in | ios::binary);

    if (!iIndexFile)
    {
        // Se não conseguiu, gerar o arquivo e abrir pra leitura
        generateCodeIndexFile();
        iIndexFile.open(this->indexFileName, ios::in | ios::binary);
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
    ofstream oIndexFile(this->indexFileName, ios::out | ios::binary);

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

    dataFile.iFile.seekg(0, dataFile.iFile.beg);
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
    // int address = binarySearch(archiveInIndex, patient_code, 0, dataFile.size());
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

