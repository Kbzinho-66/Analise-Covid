#include <iostream>
#include <vector>
#include <string>

#define HAS_CODECVT
#include "rapidcsv.h"
#include "register.hpp"
#include "cereal/cereal.hpp"

using namespace std;

int main()
{
    rapidcsv::Document doc("RS_Mini.csv", rapidcsv::LabelParams(0, 0));
    // vector<string> arrayColumn = doc.GetColumn<string>("Categoria_Nome");

    for(int i = 1; i < 49; i++)
    {
        vector<string> row = doc.GetRow<string>(i);

        int patient_code        = i;
        int patient_age         = stoi(row[0]);
        string patient_b_day    = row[1];
        string vaccination_city = row[3];
        string category_name    = row[5];

        Register *entry = new Register(patient_code, patient_age, patient_b_day, vaccination_city, category_name);
        entry->printRegister();
    }   

}