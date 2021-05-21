// Includes da STL
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Bibliotecas
#define HAS_CODECVT
#include "rapidcsv.h"
#include "archive.h"

// Nossos headers
#include "Registry.hpp"
// #include "CodeIndex.hpp"

// Nomes dos arquivos
#define ARQUIVO_BINARIO "RS_Mini.bin"
#define ARQUIVO_TEXTO "RS_Mini.csv"

using namespace std;

void generateBinaryDataFile();

void printBinaryDataFile();

void getRegistryByCode(int code);

int main() {

    generateBinaryDataFile();
    printBinaryDataFile();
    getRegistryByCode(3);

}

void generateBinaryDataFile() {

    /**
     * @brief Função para gerar o arquivo de dados binário
     * a partir de um arquivo .csv .
     * 
     * Essa função é chamada somente quando não foi possível
     * abrir um arquivo de dados binário.
     */

    rapidcsv::Document doc(ARQUIVO_TEXTO, rapidcsv::LabelParams(0, -1));

    ofstream oFile(ARQUIVO_BINARIO, ios::out | ios::binary);

    if(!oFile) {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    Archive<ofstream> archiveOut(oFile);

    for(int i = 0; i < 50; i++) {
        // Algo tá dando errado aqui, não tá lendo a última linha
        vector<string> row = doc.GetRow<string>(i);

        Registry *entry = new Registry(row);

        if (entry->patient_code == 3) {
            cout << "Posição do código 3 no arquivo: " << oFile.tellp()<<  endl;
            entry->printRegistry();
        }

        archiveOut << entry;
    }   

    oFile.close();
}

void printBinaryDataFile() {

    /**
     * @brief A princípio só mostra todos os registros
     * do arquivo pra ver se tá tudo funcionando.
     */

    Registry temp;

    ifstream iFile(ARQUIVO_BINARIO, ios::binary);

    if (!iFile) {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    Archive<ifstream> archiveIn(iFile);

    for (int i=1; i<50; i++) {
        // cout << "Posição no arquivo:" << iFile.tellg() << endl;
        archiveIn >> temp;
        // temp.printRegistry();
    }

    iFile.close();
}

void getRegistryByCode(int code) {
    /**
     * @brief Mock-up de como vai ser a função pra pegar um
     * registro dado um código
     */

    array<int, 3> addresses;
    // A posição no vetor é o código + 1, o valor, o endereço
    addresses[2] = 292;
    
    Registry temp;

    ifstream iFile(ARQUIVO_BINARIO, ios::binary);

    if (!iFile) {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    Archive<ifstream> archiveIn(iFile);

    iFile.seekg(addresses[code-1], iFile.beg); // Endereço bruto de um registro, encontrado com o tellg()

    archiveIn >> temp;
    temp.printRegistry();

    iFile.close();
}