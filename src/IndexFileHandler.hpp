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

class CodeIndex
{
    public:
        int size;
        std::string fileName;

        CodeIndex(const DataFile &dataFile);
        ~CodeIndex(void);

        void printCodeIndexFile(void);
    private:
        std::ifstream iFile;
        const DataFile &dataFile;
        void generateCodeIndexFile(void);
};

CodeIndex::CodeIndex(const DataFile &dataFile_) : dataFile(dataFile_)
{
    /**
     * @brief Construtor para a classe CodeIndex.
     * Tenta abrir um arquivo de índices. Se conseguir, abre um stream de input.
     * Se não conseguir, gera um arquivo de índices e então abre o stream.
     * 
     * @param dataFile O arquivo de dados atualmente aberto.
     */
    size = 0;
    iFile.open(ARQUIVO_INDICES, ios::in | ios::binary);

    if (!iFile)
    {
        generateCodeIndexFile();
        iFile.open(ARQUIVO_INDICES, ios::in | ios::binary);
    }
    
}

CodeIndex::~CodeIndex()
{
    iFile.close();
}

void CodeIndex::generateCodeIndexFile() 
{
    /**
     * @brief Função que vai gerar o arquivo de índices
     * especificamente para a coluna paciente_codigo.
     * 
     * @details O patient_code 1 tá no endereço 8 desse
     * arquivo. Logo, ao dar o seek, a primeira posição
     * tá em (1 * sizeof(int) * 2).
     */
    ofstream oFile(ARQUIVO_INDICES, ios::binary);

    if (!oFile) {
        cout << "Não foi possível criar o arquivo de índices." << endl;
        exit(0);
    }

    cout << "Gerando o arquivo de índices para código..." << endl;

    Archive<ifstream> archiveIn(iFile);
    Archive<ofstream> archiveOut(oFile);
    Registry temp;
    int code, position;

    for (int index = 0; index < dataFile.size(); index++) {
        /**
         * Varredura do arquivo de dados, agregando um patient_code a uma 
         * posição específica no arquivo de dados. Essa associação é 
         * então gravada no arquivo de índices como um par.
         */

        code = temp.patient_code;
        position = iFile.tellg();
        archiveOut << make_pair(code, position);
        archiveIn >> temp;
        size++;
    }

    iFile.seekg(0, iFile.beg);
    oFile.close();
}

void CodeIndex::printCodeIndexFile() {

    /**
     * @brief Printa todo o arquivo de índices
     * pra ver se tá tudo funcionando.
     */

    Archive<ifstream> archiveIn(iFile);

    pair<int, int> index;

    for (int line = 0; line < dataFile.size(); line++) {

        cout << "Posição do Índice:" << iFile.tellg();
        archiveIn >> index;
        cout << " Código: " << index.first << " Posição no arquivo de dados: " << index.second << endl;
    }

    iFile.seekg(0, iFile.beg);
}