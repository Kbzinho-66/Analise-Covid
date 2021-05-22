
/**
 * @file BinarySearchTree.cpp
 * @brief Módulo para a árvore de pesquisa binária que guardará
 *  os índices em memória
 */

#include "archive.h"
#include <iostream>
#include <fstream>

#include "DataFileHandler.hpp"
#include "Registry.hpp"

class Node
{
    /**
     * @brief Classe da estrutura de um nodo
     *  da árvore binária
     */
public:
    int address;
    Node *right;
    Node *left;
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

    void insertAddress(int address);
    void deleteAddress(int address);
    Node *search(int address);
    Node *tree_minimun(Node *leaf);
    void getRegistry(int address);

private:
    void insertAddress(int address, Node *leaf);
    void deleteAddress(int address, Node *leaf);
    Node *search(int address, Node *leaf);

    Node *root;
};

BinarySearchTree::BinarySearchTree()
{
}

BinarySearchTree::~BinarySearchTree()
{
}

void BinarySearchTree::insertAddress(int address)
{
    if (root != NULL)
    {
        insert(address, root);
    }
    else
    {
        root = new Node;
        root->address = address;
        root->left = NULL;
        root->right = NULL;
    }
}

void insert(int address, Node *leaf)
{
    if(address < leaf->address)
    {
        if (leaf->left != NULL)
        {
            insert(address, leaf->left);
        }
        else
        {
            leaf->left = new Node;
            leaf->left->address = address;
            leaf->left->left = NULL;
            leaf->left->right = NULL;
        }
    }
    else if (address >= leaf->address)
    {
        if (leaf->right != NULL)
        {
            insert(address, leaf->right);
        }
        else
        {
            leaf->right = new Node;
            leaf->right->address = address;
            leaf->right->right = NULL;
            leaf->right->left = NULL;
        }
    }
}

Node *BinarySearchTree::search(int address)
{
    return search(address, root);
}

Node *BinarySearchTree::search(int address, Node *leaf)
{
    if(leaf == NULL)
    {
        if(address == leaf->address)
        {
            return leaf;
        }

        if(address < leaf->address)
        {
            return search(address, leaf->left);
        }
        else
        {
            return search(address, leaf->right);
        }
    }
    else
    {
        return NULL;
    }
}

void BinarySearchTree::getRegistry(int address)
{
    Registry::Registry registry;
    DataFile::DataFile *dataFile = new DataFile::DataFile();

    int address = search(address)->address;

    registry = dataFile->getRegistryByAddress(address);

    registry.printRegistryInfo();
}