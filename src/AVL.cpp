#include"AVL.h"

inline void initNode(TreeNode *node, TreeNode *lchild, TreeNode *rchild, int value, int height, bool balance);

// ####################################################### public function ############################################################

// 查找某个元素，如果该元素存在则返回true，否则返回false
bool AVL::search(int value){
    TreeNode* temp = this->AVLTree;
    while(!temp){
        if(temp->value == value) return true;
        else if(temp->value > value) temp = temp->lchild;
        else temp = temp->rchild;
    }
    return false;
}

// 插入元素
bool AVL::insert(int value){
    int flag;
    if(this->AVLTree && !(flag = this->inner_search(this->AVLTree, value))) {     // 当AVL不为空时且该值已经存在
        std::cout << "The element " << value << " existed" << std::endl;
        this->clean_nstack();
        return false;
    }
    // node为新插节点  temp用来记录不平衡节点  nshead是新子树节点，即对temp平衡化后的代替temp的节点
    TreeNode* node = new TreeNode();
    initNode(node, NULL, NULL, value, DEP_INIT, BAL_INIT);
    if(!this->AVLTree) {     // 当AVL节点为空时
        this->AVLTree = node;
        return true;
    }
    if(flag == INS_L)   //左插
        this->nstack.top()->lchild = node;
    else if(flag == INS_R)               //右插
        this->nstack.top()->rchild = node;
    // std::cout << this->nstack.empty() << std::endl;
    this->balance();
    // std::cout << value << std::endl;

    return true;
}

// 获取该节点的后继节点
TreeNode* getNextNode_inorder(TreeHead *head) {
    TreeNode* temp = head;
    while(temp->lchild) temp = temp->lchild;
    return temp;
}

//重载remove函数，作为对外的接口
//因为对AVL的删除需要使用私有成员，而外部无法调用remove(TreeHead*, int)
//因此重载该函数使外部在不妨问道私有成员的情况下能够使用删除函数
bool AVL::remove(int value){
    return this->remove(this->AVLTree, value);
}

// 移除值为value的节点，移除成功返回true，否则返回false
bool AVL::remove(TreeHead *head, int value){
    if(!this->AVLTree) return false;

    int flag = this->inner_search(head, value); //判断value是否存在
    // std::cout << flag << " : " << this->nstack.top()->value << std::endl;
    if(flag != EXISTED) {
        std::cout << "Failure! The element " << value << " is not existed" << std::endl;
        this->clean_nstack();
        return false;
    }
    //value存在，进行删除
    TreeNode* node = this->nstack.top(), *pnode = NULL, *nnode = NULL;
    this->nstack.pop();
    if(!node->lchild && !node->rchild) {   //当删除的点无左右子树时
        if(this->nstack.empty()){   //删除节点为根节点
            this->AVLTree = NULL;
            delete node;
            this->balance();
            return true;
        }
        //删除节点为非根节点
        pnode = this->nstack.top();  //利用node指针读取删除节点的父节点
        if(pnode->lchild == node) pnode->lchild = NULL;
        else pnode->rchild = NULL;
        delete node;
        this->balance();
    }else if(node->lchild && node->rchild) {//如果左右子树均存在，用其后继节点代替该节点，递归删除原本后继节点的位置
        nnode = getNextNode_inorder(node->rchild);
        node->value = nnode->value;
        nnode->value = value;
        this->nstack.push(node);
        this->remove(node->rchild, value);
    }else {//只存在一棵子树，直接用子树节点代替该节点，然后删除节点
        if(this->nstack.empty()) {//删除节点为根节点
            if(node->lchild) this->AVLTree = node->lchild;
            else this->AVLTree = node->rchild;
            delete node;
            this->balance();
            return true;
        }
        //删除节点为非根节点
        pnode = this->nstack.top();
        if(pnode->lchild == node) pnode->lchild = node->lchild ? node->lchild : node->rchild;
        else pnode->rchild = node->lchild ? node->lchild : node->rchild;
        delete node;
        this->balance();
    }
    return true;
}

//对修改过后的树进行平衡操作，操作方式只需要通过nstack进行回溯即可
//在逐步回溯平衡调整的同时，既解决了树的深度更新问题，又解决了nstack的清空问题
void AVL::balance(){
    TreeNode* temp = NULL, *nshead = NULL;
    while(!this->nstack.empty()){
        temp = this->is_balance();  //判断是否平衡
            // std::cout << this->nstack.empty() << std::endl;
        if(temp){       //不平衡
            nshead = this->balance_op(temp);       //获取平衡后子树的新根节点
            if(!this->nstack.empty()){
                if(this->nstack.top()->lchild == temp) this->nstack.top()->lchild = nshead; //将就父节点的对应位置更改为新节点
                else this->nstack.top()->rchild = nshead;
            }else this->AVLTree = nshead;
            this->cal_height(nshead);
        }
    }
}

// 判断树是否平衡，如果不平衡则进行平衡操作
TreeNode* AVL::is_balance(){
    int lheight, rheight;
    TreeNode *temp = NULL;
    while(!this->nstack.empty()){
        temp = this->nstack.top();  // 获取栈顶结点
        this->nstack.pop();
        // std::cout << temp->height << std::endl;
        lheight = temp->lchild ? temp->lchild->height : 0;
        rheight = temp->rchild ? temp->rchild->height : 0;
        temp->height = (lheight > rheight ? lheight : rheight) + 1;
        if(abs(lheight - rheight) > 1) return temp; //返回非平衡节点
    }
    return NULL;    //返回NULL表示AVL没有不平衡节点
}

// 进行左旋和右旋平衡操作
//左旋，右子树节点作为根节点，原来根节点做右子树的左子树
//将原来右子树的左子树作为原来根节点的左子树
TreeNode* AVL::tLeft(TreeHead *head){
    TreeNode* nhead = head->rchild;
    head->rchild = nhead->lchild;
    nhead->lchild = head;

    return nhead;
}
//右旋，原理和左旋相反
TreeNode* AVL::tRight(TreeHead *head){
    TreeNode* nhead = head->lchild;
    head->lchild = nhead->rchild;
    nhead->rchild = head;

    return nhead;
}

#define LL 0
#define LR 1
#define RL 2
#define RR 3    // 根据以上定义，可以将L视为0，R为1，因此对应 00，01，10，11，
TreeNode* AVL::balance_op(TreeHead *head){ //对head进行平衡操作
    TreeNode *snode = NULL, *nhead = NULL;
    int flag = 0;       // 用于检测不平衡属于哪种类型
    if(this->get_subtree_height(head->lchild) < this->get_subtree_height(head->rchild)) {
        flag += 2;
        snode = head->rchild;
    }else snode = head->lchild;
    if(this->get_subtree_height(snode->lchild) < this->get_subtree_height(snode->rchild)) flag += 1;
    // std::cout << head->value << std::endl;
    switch(flag){   //根据flag进行不同的平衡调整
    case LL:
        nhead = this->tRight(head);
        break;
    case LR:
        head->lchild = this->tLeft(snode);
        nhead = this->tRight(head);
        break; 
    case RL:
        head->rchild = this->tRight(snode);
        nhead = this->tLeft(head);
        break;
    case RR:
        nhead = this->tLeft(head);
        break;
    }
    return nhead;
}

// 用于对某棵子树进行平衡调整之后，对其高度进行重新赋值
int AVL::cal_height(TreeHead *head){
    if(!head) return 0;
    int lh, rh;
    lh = this->cal_height(head->lchild); 
    rh = this->cal_height(head->rchild);
    head->height = (lh > rh ? lh : rh) + 1;
    return head->height;
}

//前序遍历
void preorder(TreeHead* head){
    if(!head) return;
    std::cout << head->value << " ";
    preorder(head->lchild);
    preorder(head->rchild);
}
// 中序遍历
void inorder(TreeHead* head){
    if(!head) return;
    inorder(head->lchild);
    std::cout << head->value << " ";
    inorder(head->rchild);
}
// 后序遍历
void postorder(TreeHead* head){
    if(!head) return;
    postorder(head->lchild);
    postorder(head->rchild);
    std::cout << head->value << " ";
}
//打印树的信息  使用flag进行选择输出的类型：前序（0）、中序（1）、后续（2）
void AVL::show(int flag){
    if(this->AVLTree == NULL){
        std::cout << "There is no element to show for you..." << std::endl;
        return;
    }
    switch(flag){
    case 0:
        std::cout << "preorder :  ";
        preorder(this->AVLTree);
        break;
    case 1:
        std::cout << "inorder :   ";
        inorder(this->AVLTree);
        break;
    case 2:
        std::cout << "postorder:  ";
        postorder(this->AVLTree);
    }
    std::cout << std::endl;
}

//该接口提供将AVL图形化的功能（只是简单的以树的形式在终端中显示）
void AVL::show_graph(){
    bool *flags = new bool[this->AVLTree->height];
    for(int i = 0;i < this->AVLTree->height;++i){
        flags[i] = false;
    }
    this->show_graph(this->AVLTree, 0, flags);
    delete flags;
}
// 图形化的实现代码
void AVL::show_graph(TreeHead *head, int height, bool *flags){
    if(!head) return;
    flags[height] = true;
    if(height) std::cout  << "__";
    std::cout << std::setw(2) << std::left << head->value;
    this->show_graph(head->lchild, height + 1, flags);
    if(!head->lchild) std::cout << std::endl;
    if(head->rchild) {
        int count = height;
        do{
            if(flags[height - count]) std::cout << " |";
            else std::cout << "  ";
            if(count > 0) std::cout << "  ";
        }while(count--);
    }
    flags[height] = false;
    this->show_graph(head->rchild, height + 1, flags);
}

// 释放平衡二叉树
AVL::~AVL(){
    this->destroy();
    if(!this->AVLTree)
        std::cout << "AVL was destroied successfully!" << std::endl;
    
}


// ####################################################### private function ############################################################

// 内部搜索函数，对路径进行存储，返回插入的位置，主要用于辅助插入功能
int AVL::inner_search(TreeHead *head, int value){
    TreeNode* temp = head;
    int flag = INS_L;
    while(temp){
        this->nstack.push(temp);
        // std::cout << "inner_search " << temp->value << std::endl;
        if(temp->value == value) return EXISTED;
        else if(temp->value > value) {
            temp = temp->lchild;
            if(flag != INS_L) flag = INS_L;
        }
        else {
            temp = temp->rchild;
            if(flag != INS_R) flag = INS_R;
        }
    }
    return flag;
}

void destroyTree(TreeHead *avlTree){
    if(avlTree == NULL) return;
    destroyTree(avlTree->lchild);
    destroyTree(avlTree->rchild);
    delete avlTree;
}

bool AVL::destroy(){
    destroyTree(this->AVLTree);
    this->AVLTree = NULL;
    return true;
}
// 清空树的路径栈
void AVL::clean_nstack(){
    while(!this->nstack.empty()){
        this->nstack.pop();
    }
}

//获取子树的高度，用于求某节点的高度或者比较某节点的左右子树高度时
int AVL::get_subtree_height(TreeHead *head){
    if(!head) return 0;
    return head->height;
}
