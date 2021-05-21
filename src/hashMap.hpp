
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
     */

    std::multimap<std::string, int>::iterator it;

    for (it = map.begin(); it != map.end(); it++) 
    {
        if(it -> first == dateKey)
        {
            int dataPostion = it -> second;
            // acessa arquivo com dataPostion
        }
    }
}

