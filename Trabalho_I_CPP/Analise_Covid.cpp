#include <iostream>
#include <vector>
#include <string>
#include <fstream>


#define HAS_CODECVT
#include "rapidcsv.h"
#include "Registry.hpp"
#include "archive.h"

using namespace std;

int main() {

    int patient_code, patient_age;
    string patient_b_day;

    // Escrita no arquivo binário
    rapidcsv::Document doc("RS_Mini.csv", rapidcsv::LabelParams(0, 0));
    // vector<string> arrayColumn = doc.GetColumn<string>("Categoria_Nome");

    ofstream oFile("RS_Mini.bin", ios::out | ios::binary);

    if(!oFile) {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    Archive<ofstream> a(oFile);

    for(int i = 0; i < 50; i++) {
        // Algo tá dando errado aqui, não tá lendo a última linha
        vector<string> row = doc.GetRow<string>(i);

        patient_code        = i + 1; // a GetRow pula o header automaticamente, então o código de paciente 1 é a linha 0
        patient_age         = stoi(row[0]);
        patient_b_day    = row[1];
        string vaccination_city = row[3];
        string category_name    = row[5];

        Registry *entry = new Registry(patient_code, patient_age, patient_b_day, vaccination_city, category_name);
        // entry->printRegistry();

        // cout << "Escrevendo no arquivo..." << endl;
        a << entry;
    }   

    oFile.close();

    // Leitura do arquivo binário
    ifstream iFile("RS_Mini.bin", ios::binary);

    if (!iFile) {
        cout << "Não foi possível abrir o arquivo" << endl;
        exit(0);
    }

    Archive<ifstream> a2(iFile);
    for (int i=1; i<50; i++) {
        a2 >> patient_code >> patient_age >> patient_b_day;
        cout << "Código: " << patient_code << endl;
        cout << "Idade: " << patient_age << endl;
        cout << "Dt. Nascimento: " << patient_b_day << endl;
        cout << endl;
    }

    iFile.close();
}