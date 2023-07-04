#include<iostream>
#include"../include/opavl.h"


int main(int argc, char* argv[]){
    std::string filename;

    if(argc == 2) filename = argv[1];
    else {
        std::cout << "datafile is not input!!\n";
        return 0;
    }

    std::ifstream ifs;
    ifs.open(filename, std::ios::in);
    if(!ifs.is_open()) {
        std::cout << "Input is empty!" << std::endl;
        return 0;
    }

    AVL *avltree = AVLTree(ifs);
    
    std::cout << "AVL tree show:" << std::endl;
    avltree->show_graph();


    std::cout << "AVL elements show:" << std::endl;
    showElems(avltree, PREORDER);
    showElems(avltree, INORDER);
    std::cout << std::endl;

    srand((unsigned)time(NULL));
    int be_removing = rand() % 100;    //该变量为测试之用，可以自行变成设置
    std::cout << "trying to remove the element " << be_removing << ":" << std::endl;
    removeElem(avltree, be_removing);
    showElems(avltree, PREORDER);
    showElems(avltree, INORDER);

    release_sources(avltree);
    
    return 0;
}