#ifndef NODELIST_H
#define NODELIST_H

#include "node.h"
#include <vector>

class NodeList {
public:
    // 默认构造函数，初始化链表为空
    NodeList() ;
    // 带参数构造函数，根据 std::vector<Node> 创建链表
    NodeList(const std::vector<Node>& vectorNodes);
    //析构函数
    ~NodeList();

    //插入node
    void addNode(Node* newNode);
    //删除node
    bool deleteNodeByName(const QString &name);
    //show链表所有节点
    void displayList() const;
    //根据喜好程度对链表进行排序（升序）
    void sortListByRating();
    // 获取链表的头节点
    Node* getHead() const { return head; }

private:
    //定义 成员变量
    Node* head;

    //函数声明（用来实现排序的，最后都只会被sortList调用）
    Node* mSort(Node* head);
    Node* merge(Node* left, Node* right);
    Node* getMiddle(Node* head);

};

#endif // NODELIST_H
