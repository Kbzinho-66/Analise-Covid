
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
#include "Registry.hpp"

class HashMap
{
    public:
        std::multimap<std::string, int> map;
        HashMap();
        ~HashMap();
        void insertNewValue(std::string dateKey, int entryPosition);
        void printAllValuesFromKey(std::string dateKey);
};


HashMap::HashMap() 
{

}

void HashMap::insertNewValue(std::string dateKey, int entryPosition)
{
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

    if(map.find(dateKey) == map.end())
    {
        std::cout << "Data não encontrada" << std::endl;
        return;
    }

    for (it = map.begin(); it != map.end(); it++) 
    {
        if(it -> first == dateKey)
        {
            int address = it -> second;
            Registry::Registry *registry = dataFile->getRegistryByAddress(address);

            registry->printRegistryInfo();
        }
    }
}

