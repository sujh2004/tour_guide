#include "nodelist.h"
#include <QDebug>
#include <algorithm> // for std::sort


// 默认构造函数，初始化链表为空
NodeList::NodeList() : head(nullptr) {}

// 带参数构造函数，根据 std::vector<Node> 创建链表
NodeList::NodeList(const std::vector<Node>& vectorNodes) : head(nullptr) {
    for (const Node& node : vectorNodes) {
        Node* newNode = new Node(node);  // 复制每一个 Node
        addNode(newNode);
    }
}

// 析构函数：销毁链表并释放内存
NodeList::~NodeList() {
    Node* current = head;
    while (current) {
        Node* next = current->getNext(); // 保留下一个节点
        delete current;                   // 删除当前节点
        current = next;                   // 移动到下一个节点
    }
}

//添加节点
void NodeList::addNode(Node* newNode){
    //首先判断链表是否为空 空就直接head=newNode 这里因为是NodeList的方法，自动获取对应变量
    if(!head){
        head = newNode;
    }else{
        //用current指向head 找到链表结尾
        Node *current = head;
        while(current->getNext()!=nullptr){
            current = current->getNext();
        }
        //已到达最后一个node 把newNode link上去
        current ->setNext(newNode);
    }
}

//根据景点名称删收藏
bool NodeList::deleteNodeByName(const QString &name){
    Node* current = head;//current负责查找指定node
    Node *prev = NULL;//prev保存current的前一个节点

    //while逐个逐个check链表的node
    while(current){
        if(current->getName() == name){//找到要被删的节点
            if(prev){//判断是不是头节点，确保prev存在
                prev->setNext(current->getNext());
            }else{//被删节点是头结点
                head = current->getNext();
            }
            delete current;
            return true;//退出函数
        }
        //如果目前current不是要删的 更新prev和current
        prev = current;
        current = current->getNext();
    }
    return false;//while下来没找到
}

//显示所有节点
void NodeList::displayList() const{
    Node *current = head;//用current遍历
    while(current){
        qDebug()<<"ID:"<<current->getId()<<",Name:"<<current->getName()<<",Rating:"<<current->getRating();
        current = current->getNext();//更新current
    }
}

//---------归并排序--------
//获取链表中间节点
Node *NodeList::getMiddle(Node* head){//用快慢指针法::slow 指针每次走一步 fast 指针每次走两步。当 fast 到达链表尾部时，slow 就正好在中间。
    if(!head)return nullptr;//链表为空
    Node* fast = head->getNext();
    Node* slow = head;

    while(fast && fast->getNext()){
        slow = slow->getNext();//慢指针走一步
        fast = fast->getNext()->getNext();//快指针走两步
    }
    return slow;
}

Node* NodeList::merge(Node* left, Node* right){
    Node dummy;//結果鏈表的虛擬開頭
    Node *tail = &dummy;

    while(left &&right){
        //從rating低到高排序 如果rating same就看名字升序
        //左rate小或者rate same左名小
        if(left->getRating()<right->getRating() ||
            (left->getRating()==right->getRating() && left->getName()<right->getName())){
            tail->setNext(left);
            left = left->getNext();
        }else{
            tail->setNext(right);
            right = right->getNext();
        }
        tail = tail->getNext();
    }

    //处理剩下没加完的left right
    tail->setNext(left ? left : right);
    // 返回合并后的链表头
    return dummy.getNext();
}

Node* NodeList::mSort(Node* nodeHead) {
    //如果链表只有一个节点
    if (!nodeHead || !nodeHead->getNext())
        return nodeHead;

    Node* mid = getMiddle(nodeHead);
    Node* rightHead = mid->getNext();
    mid->setNext(nullptr);  // 断开中间节点
    //递归
    Node* left = mSort(nodeHead);
    Node* right = mSort(rightHead);

    return merge(left, right);
}

//public的sortListByRating
void NodeList::sortListByRating() {
    head = mSort(head);
}
