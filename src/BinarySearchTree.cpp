
/**
 * @file BinarySearchTree.cpp
 * @brief Módulo para a árvore de pesquisa binária que guardará
 *  os índices em memória
 */

#include "archive.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "DataFileHandler.hpp"
#include "IndexFileHandler.hpp"
#include "Registry.hpp"

class Node
{
    /**
     * @brief Classe da estrutura de um nodo
     *  da árvore binária
     */
public:
    std::string city;
    vector<int> indexes;
    Node *left;
    Node *right;
};

class BinarySearchTree
{
    /**
     * @brief Classe da estrutura da árvore
     *  binária
     */
public:
    BinarySearchTree();
    ~BinarySearchTree();

    void insertIndex(std::string city, int index);
    Node *searchCity(std::string city);
    bool hasRegistryIndex(int index);
    void getRegistry(int index);
    void printCitiesAndTotalVaccinations();
	void inorderPrint();

private:
    void insertIndex(std::string city, int index, Node *leaf);
    Node *searchCity(std::string city, Node *leaf);
    bool searchRegistry(int index, Node *leaf);
    bool indexAlreadyInVector(int target, vector<int> indexes);
    void printCitiesAndTotalVaccinations(Node *leaf);
	void inorderPrint(Node *leaf);


    Node *root;
};

BinarySearchTree::BinarySearchTree()
{
}

BinarySearchTree::~BinarySearchTree()
{
}

void BinarySearchTree::insertIndex(std::string city, int index)
{
    /**
     * @brief Funcão de inserção do índice na árvore. Se cidade já
     * existir na árvore, adiciona o índice no array de indices do nodo.
     * 
     * @param city String da cidade do registro do índice
     * @param index Índice do registro a ser inserido
     * 
     */
    if (root != NULL)
    {
        insertIndex(city, index, root);
    }
    else
    {
        root = new Node;
        root->city = city;
        root->indexes.push_back(index);
        root->left = NULL;
        root->right = NULL;
    }
}

void BinarySearchTree::insertIndex(std::string city, int index, Node *leaf)
{
    /**
     * @brief Função recursiva que irá realizar a inserção 
     * de um novo nodo na árvore ou pela inserção do índice na
     * lista do nodo caso cidade já esteja presente
     * 
     * @param city A cidade do registro que se deseja inserir
     * @param index O índice do registro para ser inserido
     * @param lead Nodo atual
     */
    if (city.compare(leaf->city) < 0)
    {
        if (leaf->left != NULL)
        {
            insertIndex(city, index, leaf->left);
        }
        else
        {
            leaf->left = new Node;
            leaf->left->city = city;
            leaf->left->indexes.push_back(index);
            leaf->left->left = NULL;
            leaf->left->right = NULL;
        }
    }
    else if (city.compare(leaf->city) > 0)
    {
        if (leaf->right != NULL)
        {
            insertIndex(city, index, leaf->right);
        }
        else
        {
            leaf->right = new Node;
            leaf->right->city = city;
            leaf->right->indexes.push_back(index);
            leaf->right->right = NULL;
            leaf->right->left = NULL;
        }
    }
    else if (city.compare(leaf->city) == 0)
    {
        // se cidade já existe na árvore só adiciona o índice
        if (!indexAlreadyInVector(index, leaf->indexes))
            leaf->indexes.push_back(index);
    }
}

Node *BinarySearchTree::searchCity(std::string city)
{
    /**
     * @brief Função responsavel pela busca do nodo com valor
     * de dada idade
     *
     * @param city String da cidade que se deseja buscar na árvore
     * 
     * @return O nodo da cidade encontrada ou nulo caso não encontrado
     */
    return searchCity(city, root);
}

Node *BinarySearchTree::searchCity(std::string city, Node *leaf)
{
    /**
     * @brief Função que realiza busca em profundidade para 
     * buscar nodo com valor de dada cidade
     *
     * @param city String da cidade que se deseja buscar na árvore
     * @param leaf nodo atual
     * 
     * @return O nodo da cidade encontrada ou nulo caso nodo
     * não exista
     */
    if (leaf != NULL)
    {
        if (city == leaf->city)
        {
            return leaf;
        }

        if (city < leaf->city)
        {
            return searchCity(city, leaf->left);
        }
        else
        {
            return searchCity(city, leaf->right);
        }
    }
    else
    {
        return NULL;
    }
}

bool BinarySearchTree::hasRegistryIndex(int index)
{
    /**
     * @brief Função responsavel por dizer se dado índice existe na árvore
     *
     * @param index índice que se deseja verificar se está na lista
     * 
     * @return true se existe, falso se não
     */
    return searchRegistry(index, root);
}

bool BinarySearchTree::searchRegistry(int index, Node *leaf)
{
    /**
     * @brief Função responsável por busca em profundidade na árvore
     * em busca do índice do registro
     * 
     * @param index índice que se deseja verificar se está na lista
     * @param leaf nodo atual
     * 
     * @return true se existe, falso se não
     */
    if (leaf != NULL)
    {
        // se achou o valor em um dos nodos, retorna verdadeiro
        if (std::find(leaf->indexes.begin(), leaf->indexes.end(), index) != leaf->indexes.end())
        {
            return true;
        }

        // se não, dispara busca nos dois nodos da árvore
        return (searchRegistry(index, leaf->left) || searchRegistry(index, leaf->right));
    }
    else
    {
        // quando atingir o fundo e nodo for nulo, retorna falso pq não achou
        return false;
    }
}

void BinarySearchTree::getRegistry(int index)
{
    Registry registry;

    if (hasRegistryIndex(index))
    {
        //abre arquivo da cidade
        DataFile *dataFile = new DataFile();
        CodeIndex *codeIndex = new CodeIndex(*dataFile);
        //procura o indice
        codeIndex->searchRegistryByCode(index);

        return;
    }

    else
        cout << "Código não encontrado." << endl
             << endl;
}

bool BinarySearchTree::indexAlreadyInVector(int target, vector<int> indexes)
{
    for (auto &index : indexes)
        if (index == target)
            return true;

    return false;
}

void BinarySearchTree::printCitiesAndTotalVaccinations(){
	printCitiesAndTotalVaccinations(root);
	cout << "\n";
}

void BinarySearchTree::printCitiesAndTotalVaccinations(Node *leaf){
	if(leaf != NULL){
		cout << leaf->city << ", " << leaf->indexes.size() << " vacinados;" << endl;
		inorderPrint(leaf->left);
		inorderPrint(leaf->right);
	}
}

void BinarySearchTree::inorderPrint(){
	inorderPrint(root);
	cout << "\n";
}

void BinarySearchTree::inorderPrint(Node *leaf){
	if(leaf != NULL){
		inorderPrint(leaf->left);
		cout << leaf->city << "," << leaf->indexes.size() << ";" << endl;
		inorderPrint(leaf->right);
	}
}