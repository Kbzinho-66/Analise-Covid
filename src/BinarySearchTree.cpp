#include "archive.h"
#include <iostream>
#include <fstream>

#include "DataFileHandler.hpp"
#include "Registry.hpp"


class Node
{
    public:
        int address;
        Node *right;
        Node *left;
};

class BinarySearchTree
{
    public:
        BinarySearchTree();
        ~BinarySearchTree();

        void insertAddress(int address);
        void deleteAddress(int address);
        int getAddress();
};