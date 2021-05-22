#pragma once

#include <vector>

#define HAS_CODECVT
#include "rapidcsv.h"

#define ARQUIVO_TEXTO "RS_Mini.csv"

using namespace std;

class Reader
{
    /**
     * @brief Classe responsável pela leitura
     *  dos dados de um arquivo CSV
     */
public:
    string fileName;
    rapidcsv::Document *doc;
    vector<string> getRow(int index);

    Reader(void);
    Reader(string fileName);
};

Reader::Reader() 
{
    this->fileName = ARQUIVO_TEXTO;

    // Parâmetros 0 e -1 indicam que o a linha 0 contém o label de cada coluna e que 
    // nenhuma coluna é o label de cada linha 
    // (embora a coluna 0 seja, mas aí não tem como acessar o valor dela)
    this->doc = new rapidcsv::Document(this->fileName, rapidcsv::LabelParams(0, -1));
}

Reader::Reader(string fileName)
{
    this->fileName = fileName;

    this->doc = new rapidcsv::Document(this->fileName, rapidcsv::LabelParams(0, -1));
}

vector<string> Reader::getRow(int index)
{
    /**
     * @brief Método responsável por buscar uma linha
     *  do csv e retornar um array com seus valores
     */

    vector<string> row = doc->GetRow<string>(index);

    return row;
}