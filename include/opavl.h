#ifndef OPAVL_H
#define OPAVL_H
#include"AVL.h"
#include<string>
#include<fstream>

#define PREORDER 0
#define INORDER 1
#define POSTORDER 2

AVL* AVLTree(std::ifstream &ifs);

void showElems(AVL* avltree, int flag);

void removeElem(AVL* avltree, int value);

AVL* release_sources(AVL* avltree);

#endif