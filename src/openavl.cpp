#include "opavl.h"

// 通过文件创建AVL
AVL* AVLTree(std::ifstream &ifs){
    AVL* avl = new AVL();
    int value;
    while(ifs >> value){
        avl->insert(value);
    }
    return avl;
}

void removeElem(AVL* avltree, int value){
    avltree->remove(value);
}


// 前序、中序或后续打印AVL存储的值
void showElems(AVL* avltree, int flag){
    avltree->show(flag);
}

AVL* release_sources(AVL* avltree){
    if(avltree){
        delete avltree;
    }
    avltree = NULL;
    return avltree;
}