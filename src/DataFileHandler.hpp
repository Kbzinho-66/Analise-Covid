#include "archive.h"
#include <iostream>
#include <fstream>
#include "Reader.hpp"

#define ARQUIVO_BINARIO "RS_Mini.bin"

using namespace std;

class DataFile
{
public:
    void generateBinaryDataFile();
    void printBinaryDataFile();
    Registry getRegistryByAddress(const int address);

    ofstream oFile;

    DataFile();
    ~DataFile();
};

DataFile::DataFile()
{
}

void DataFile::generateBinaryDataFile()
{
    /**
     * @brief Função para gerar o arquivo de dados binário
     * a partir de um arquivo .csv .
     * 
     * Essa função é chamada somente quando não foi possível
     * abrir um arquivo de dados binário.
     */

    Reader *reader = new Reader("RS_Mini.csv");

    ofstream oFile(ARQUIVO_BINARIO, ios::out | ios::binary);

    if (!oFile)
    {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    Archive<ofstream> archiveOut(oFile);

    for (int i = 0; i < 50; i++)
    {
        // Algo tá dando errado aqui, não tá lendo a última linha
        vector<string> row = reader->getRow(i);
        Registry *entry = new Registry(row);

        if (entry->patient_code == 3)
        {
            cout << "Posição do código 3 no arquivo: " << oFile.tellp() << endl;
            entry->printRegistry();
        }

        archiveOut << entry;
    }

    oFile.close();
}