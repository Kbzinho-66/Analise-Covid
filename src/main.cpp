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
#include "Reader.hpp"
#include "DataFileHandler.hpp"

// Nomes dos arquivos
#define ARQUIVO_BINARIO "RS_Mini.bin"
#define ARQUIVO_TEXTO "RS_Mini.csv"
#define ARQUIVO_INDICES "Indices_Codigo_RS.bin"

using namespace std;

void generateCodeIndexFile();

void printCodeIndexFile();

int main() {

    DataFile *dataFile = new DataFile();
    // dataFile->printBinaryDataFile();
    // generateCodeIndexFile();
    // printCodeIndexFile();
}

//TODO (Mover tudo isso pra um arquivo específico)
// void generateCodeIndexFile() 
// {
//     /**
//      * @brief Função que vai gerar o arquivo de índices
//      * especificamente para a coluna paciente_codigo.
//      * 
//      * @details O patient_code 1 tá no endereço 8 desse
//      * arquivo. Logo, ao dar o seek, a primeira posição
//      * tá em (1 * sizeof(int) * 2).
//      */

//     Registry temp;
//     int elements = 0;
//     int code, position;

//     ifstream iFile(ARQUIVO_BINARIO, ios::binary);

//     if (!iFile) {
//         cout << "Não foi possível abrir o arquivo de dados." << endl;
//         exit(0);
//     }
//     Archive<ifstream> archiveIn(iFile);

//     ofstream oFile(ARQUIVO_INDICES, ios::binary);

//     if (!oFile) {
//         cout << "Não foi possível abrir o arquivo de índices." << endl;
//         exit(0);
//     }
//     Archive<ofstream> archiveOut(oFile);

//     for (int i=1; i<=dataFile.size(); i++) {
//         /**
//          * Varredura do arquivo de dados, agregando
//          * um patient_code a uma posição específica
//          * no arquivo binário. Essa associação é 
//          * gravada no arquivo de índices.
//          */

//         code = temp.patient_code;
//         position = iFile.tellg();
//         archiveOut << make_pair(code, position);
//         archiveIn >> temp;
//         elements++;
//     }

//     // cout << "Foram armazenados " << elements << " índices de códigos." << endl;

//     iFile.close();
//     oFile.close();
// }

// void printCodeIndexFile() {

//     /**
//      * @brief Printa todo o arquivo de índices
//      * pra ver se tá tudo funcionando.
//      */

//     ifstream iFile(ARQUIVO_INDICES, ios::binary);

//     if (!iFile) {
//         cout << "Não foi possível abrir o arquivo de dados." << endl;
//         exit(0);
//     }
//     Archive<ifstream> archiveIn(iFile);

//     pair<int, int> index;

//     for (int i=0; i<dataFile.size(); i++) {

//         cout << "Índice:" << iFile.tellg();
//         archiveIn >> index;
//         cout << " Código: " << index.first << " Posição: " << index.second << endl;
//     }

//     iFile.close();
// }