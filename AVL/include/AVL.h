#pragma once
#include<iostream>
#include<stack>
#include<iomanip>


#ifndef AVL_H
#define AVL_H

#define DEP_INIT 1
#define BAL_INIT 0

#define EXISTED 0
#define INS_L 1
#define INS_R 2

typedef struct Node{
    struct Node* lchild;
    struct Node* rchild;
    int value;
    int height;
    bool balance;
}TreeNode, TreeHead;

inline void initNode(TreeNode *node, TreeNode *lchild, TreeNode *rchild, int value, int height, bool balance){
    if(node == NULL) return;
    node->lchild = lchild;
    node->rchild = rchild;
    node->value = value;
    node->height = height;
    node->balance = balance;
}

class AVL{
public:
    AVL():AVLTree(NULL){}

    bool search(int value);

    bool insert(int value);

    bool remove(int value);


    void balance();

    TreeNode* is_balance();

    TreeNode* tLeft(TreeHead *head);
    TreeNode* tRight(TreeHead *head);

    TreeNode* balance_op(TreeHead *head);

    // TreeNode* spin_subtree(TreeHead* th);

    int cal_height(TreeHead *head);

    void show(int flag);
    
    void show_graph();      //该接口提供将AVL图形化的功能（只是简单的以树的形式在终端中显示）

    ~AVL();

private:

    int inner_search(TreeHead *head, int value);

    void show_graph(TreeHead *head, int height, bool *);

    bool remove(TreeHead *head, int value);
    
    bool destroy();

    void clean_nstack();

    int get_subtree_height(TreeHead *head);
    
    TreeHead* AVLTree;
    std::stack<TreeNode*> nstack;
};

#endif