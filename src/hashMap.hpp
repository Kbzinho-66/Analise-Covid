
/**
 * @file hashMap.hpp
 * @author Marcello Fabrizio
 * @brief Módulo responsável pela árvore que vai indexar os 
 *        as posições "fisicas" das entradas no arquivo de dados
 *        pela data de vacinação.
 */

#include <map>
#include <iostream>
#include <cassert>

#include "DataFileHandler.hpp"
#include "IndexFileHandler.hpp"
#include "Registry.hpp"

class HashMap
{
public:
    std::multimap<std::string, int> map;
    HashMap();
    ~HashMap();
    void insertNewValue(std::string dateKey, int entryPosition);
    void printAllValuesFromKey(std::string dateKey);
    void searchRegistry(int index);
};

HashMap::HashMap()
{
}

void HashMap::insertNewValue(std::string dateKey, int entryPosition)
{
    //insert já verifica se valor chave já está na lista.
    //se está, insere no conjunto dos valores, se não insere
    //nova chave com o valor
    map.insert(std::pair<std::string, int>(dateKey, entryPosition));
}

void HashMap::printAllValuesFromKey(std::string dateKey)
{

    /**
     * @brief Imprime todos os valores de dada
     *  chave. Acessa o arquivo de dados para 
     *  recuperar os dados.
     * 
     * @param dateKey A data que serve como chave
     */

    DataFile::DataFile *dataFile = new DataFile();

    std::multimap<std::string, int>::iterator it;

    if (map.find(dateKey) == map.end())
    {
        std::cout << "Data não encontrada" << std::endl;
        return;
    }

    for (it = map.begin(); it != map.end(); it++)
    {
        if (it->first == dateKey)
        {
           getRegistry(it->second);
        }
    }
}

void HashMap::searchRegistry(int index)
{
    std::multimap<std::string, int>::iterator it;

    for (it = map.begin(); it != map.end(); it++)
    {
        if (it->second == index)
        {
            getRegistry(index);
            return;
        }
    }

    cout << "Código não encontrado." << endl
         << endl;
}

void getRegistry(int index)
{
    //abre arquivo da cidade
    DataFile *dataFile = new DataFile();
    CodeIndex *codeIndex = new CodeIndex(*dataFile);
    //procura o indice
    codeIndex->searchRegistryByCode(index);
}