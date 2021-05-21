#include <vector>

#define HAS_CODECVT
#include "rapidcsv.h"

using namespace std;

class Reader
{
    /**
     * @brief Classe responsável pela leitura
     *  dos dados de dado arquivo CSV
     */
public:
    string filePath;
    rapidcsv::Document *doc;
    vector<string> getRow(int index);

    Reader(string filePath);
};

Reader::Reader(string filePath)
{
    this->filePath = filePath;
    this->doc = new rapidcsv::Document(this->filePath, rapidcsv::LabelParams(0, -1));
}

vector<string> Reader::getRow(int index)
{
    /**
     * @brief Método responsável por buscar uma linha
     *  do cvs e retornar um array com seus valores
     */

    vector<string> row = doc->GetRow<string>(index);

    return row;
}