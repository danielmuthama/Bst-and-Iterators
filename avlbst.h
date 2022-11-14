ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "bst.h"

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a balance of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
    : Node<Key, Value>(key, value, parent),
      balance_(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    // Methods for inserting/removing elements from the tree. You must implement
    // both of these methods.
    virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
    virtual void erase(const Key& key);

private:
    /* Helper functions are strongly encouraged to help separate the problem
       into smaller pieces. You should not need additional data members. */

    /* You should write these helpers for sure.  You may add others. */
    void rotateLeft (AVLNode<Key, Value> *n);
    void rotateRight (AVLNode<Key, Value> *n);
    void insertFix(AVLNode<Key, Value> *parent, AVLNode<Key, Value>* child);
    AVLNode<Key, Value>* getSuccessor(AVLNode<Key, Value>* node);
    void removeFix(AVLNode<Key, Value> *n, int diff);

    /* A provided helper function to swap 2 nodes location in the tree */
    void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);
};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
    //create a new node
    AVLNode<Key,Value>* new_node = new AVLNode<Key,Value>(keyValuePair.first, keyValuePair.second, NULL);
    new_node->setBalance(0);   
    new_node->setRight(NULL);
    new_node->setLeft(NULL);

    if (this->mRoot == NULL) {
        this->mRoot = new_node;
        return;
    }

    AVLNode<Key,Value> *parent = NULL;
    AVLNode<Key,Value>* next = static_cast<AVLNode<Key,Value>*>(this->mRoot);

    while (true){
        parent = next;
        if (keyValuePair.first  == parent->getKey()){
            parent->setValue(keyValuePair.second);
            return;
        }
        else if (keyValuePair.first < parent->getKey()) {
            if (parent->getLeft() == NULL) {
                parent->setLeft(new_node);
                new_node->setParent(parent);
                break;
            }
            next = parent->getLeft();
        } 
        else {
            if (parent->getRight() == NULL) {
                parent->setRight(new_node);
                new_node->setParent(parent);
                break;
            }
            next = parent->getRight();
        }
    }

    if (parent->getBalance() == -1 || parent->getBalance() == 1) {
        parent->setBalance(0);
        return;
    }
    else {
        if (parent->getLeft() == new_node){
            parent->setBalance(-1);
        }
        else {
            parent->setBalance(1);
        }
        insertFix(parent, new_node);
    }

}
template<typename Key, typename Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value> *parent, AVLNode<Key, Value>* child)
 {
    // parent and grandparent should not be NULL
    if (parent == NULL || parent->getParent() == NULL) {
        return;
    }

    AVLNode<Key, Value> *grandparent = parent->getParent();

    if (parent == grandparent->getLeft()) { // left child of grandparent
        grandparent->setBalance(grandparent->getBalance() - 1);

        if (grandparent->getBalance() == 0) {
            return; //already balanced
        }

        if (grandparent->getBalance() == -1) {
            insertFix(grandparent, parent);
            return;
        }

        // grandparent balance is -2
        if (child == parent->getLeft()) {
            rotateRight(grandparent);
            parent->setBalance(0);
            grandparent->setBalance(0);

        } 
        else {
            rotateLeft(parent);
            rotateRight(grandparent);

            if (child->getBalance() == -1) {
                parent->setBalance(0);
                grandparent->setBalance(1);

            } else if (child->getBalance() == 0) {
                parent->setBalance(0);
                grandparent->setBalance(0);

            } else {
                parent->setBalance(-1);
                grandparent->setBalance(0);
            }
            child->setBalance(0);
        }

    } 
    else { // right child of grandparent
        grandparent->setBalance(grandparent->getBalance() + 1);

        if (grandparent->getBalance() == 0) {
            return; // nothing to do, it is already balanced
        }

        if (grandparent->getBalance() == 1) {
            insertFix(grandparent, parent);
            return;
        }

        // grandparent balance is 2
        if (child == parent->getRight()) { // zig-zig - only 1 rotation
            rotateLeft(grandparent);
            parent->setBalance(0);
            grandparent->setBalance(0);

        } 
        else { // zig-zag - 2 rotations
            rotateRight(parent);
            rotateLeft(grandparent);

            if (child->getBalance() == 1) {
                parent->setBalance(0);
                grandparent->setBalance(-1);
            } 
            else if (child->getBalance() == 0) {
                parent->setBalance(0);
                grandparent->setBalance(0);

            } 
            else {
                parent->setBalance(1);
                grandparent->setBalance(0);
            }
            child->setBalance(0);
        }
    }
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getSuccessor(AVLNode<Key, Value>* node) 
{
    if (node->getRight() != NULL) {
        node = node->getRight();
        while (node->getLeft() != NULL) {
            node = node->getLeft();
        }
        return node;
    }
    else{
        AVLNode<Key, Value>* parent = node->getParent();
        while(parent != NULL && node == parent->getRight()){
            node = parent;
            parent = parent->getParent();
        }
        return parent;
    }
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished.
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::erase(const Key& key)
{
    AVLNode<Key, Value>* node = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));

    if (node == NULL) {
        return;  // the value is not in the BST
    }

    if (node->getLeft() != NULL && node->getRight() != NULL) {
        AVLNode<Key, Value>* successor = getSuccessor(node);
        nodeSwap(node, successor);
    }

    AVLNode<Key, Value> *child = node->getLeft();
    if (node->getRight() != NULL) {
        child = node->getRight();
    }

    AVLNode<Key, Value>* parent = node->getParent();
    if (child != NULL){
        child->setParent(parent);
    }

    int diff;
    if (parent == NULL) {
        this->mRoot = child;
    } 
    else {
        if (node == parent->getLeft()) {
            parent->setLeft(child);
            diff = 1;
        } 
        else {
            parent->setRight(child);
            diff = -1;
        }
    }

    // delete node
    delete node;

    removeFix(parent, diff);
}
template<typename Key, typename Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int diff)
{
    if (n == NULL){
        return;
    }

    AVLNode<Key, Value>* p = n->getParent();
    AVLNode<Key, Value>* c;

    int ndiff = -1;
    if (p != NULL && n==p->getLeft()){
        ndiff = 1;
    }
    //negative
    if (n->getBalance() + diff == -2){
        //zig zig
        c = n->getLeft();
        if (c->getBalance() == -1 || c->getBalance() == 0){
            rotateRight(n);
            if(c->getBalance() == -1){
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p,ndiff);
            }
            if(c->getBalance() == 0){
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
        }
        else{
            AVLNode<Key, Value>* g = c->getRight();
            rotateLeft(c);
            rotateRight(n);
            if (g->getBalance() == 1){
                n->setBalance(0);
                c->setBalance(-1);
                g->setBalance(0);
            }
            if(g->getBalance() == 0){
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            if (g->getBalance() == 1){
                n->setBalance(1);
                c->setBalance(0);
                g->setBalance(0);
            }
            removeFix(p,ndiff);
        }
    }
    else if (n->getBalance() == 0){
        n->setBalance(n->getBalance() + diff);
        return;
    }
    else{
        n->setBalance(0);
        removeFix(p,ndiff);
    }

    //negative
    if (n->getBalance() + diff == 2){
        //zig zig
        c = n->getRight();
        if (c->getBalance() == 1 || c->getBalance() == 0){
            rotateLeft(n);
            if(c->getBalance() == 1){
                n->setBalance(0);
                c->setBalance(0);
                removeFix(p,ndiff);
            }
            if(c->getBalance() == 0){
                n->setBalance(1);
                c->setBalance(-1);
                return;
            }
        }
        else{//zig zag
            AVLNode<Key, Value>* g = c->getLeft();
            rotateRight(c);
            rotateLeft(n);            
            if (g->getBalance() == -1){
                n->setBalance(0);
                c->setBalance(1);
                g->setBalance(0);
            }
            if(g->getBalance() == 0){
                n->setBalance(0);
                c->setBalance(0);
                g->setBalance(0);
            }
            if (g->getBalance() == -1){
                n->setBalance(-1);
                c->setBalance(0);
                g->setBalance(0);
            }
            removeFix(p,ndiff);
        }
    }
    else if (n->getBalance() == 0){
        n->setBalance(n->getBalance() + diff);
        return;
    }
    else{
        n->setBalance(0);
        removeFix(p,ndiff);
    }


}

/**
* Rotates n down and to the left
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateLeft (AVLNode<Key, Value> *n)
{
    AVLNode<Key, Value>* y = n->getRight();
    AVLNode<Key, Value>* rootParent = n->getParent();
    y->setParent(rootParent);

    //set the root parent
    if (n->getParent() == NULL) {        
        this->mRoot = n->getRight();
    }
    else if (rootParent->getRight() == n){
        rootParent->setRight(y);
    }
    else{
        rootParent->setLeft(y);
    }    

    //pointer shifts
    AVLNode<Key, Value>* c = y->getLeft();

    y->setLeft(n);
    n->setParent(y);
    n->setRight(c);
    if (c != NULL){
        c->setParent(n);
    }
    
}

/**
* Rotates n down and to the right
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::rotateRight (AVLNode<Key, Value> *n)
{
    AVLNode<Key, Value>* y = n->getLeft();
    AVLNode<Key, Value>* rootParent = n->getParent();

    y->setParent(rootParent);
    if (n->getParent() == NULL) {        
        this->mRoot = y;
    }
    else if (rootParent->getRight() == n){
        rootParent->setRight(y);
    }
    else{
        rootParent->setLeft(y);
    }    

    AVLNode<Key, Value>* c = y->getRight();

    y->setRight(n);
    n->setParent(y);
    n->setLeft(c);
    if (c != NULL){
        c->setParent(n);
    }
}

/**
 * Given a correct AVL tree, this functions relinks the tree in such a way that
 * the nodes swap positions in the tree.  Balances are also swapped.
 */
template<typename Key, typename Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    if((n1 == n2) || (n1 == NULL) || (n2 == NULL) ) {
        return;
    }
    Node<Key, Value>* n1p = n1->getParent();
    Node<Key, Value>* n1r = n1->getRight();
    Node<Key, Value>* n1lt = n1->getLeft();
    bool n1isLeft = false;
    if(n1p != NULL && (n1 == n1p->getLeft())) n1isLeft = true;
    Node<Key, Value>* n2p = n2->getParent();
    Node<Key, Value>* n2r = n2->getRight();
    Node<Key, Value>* n2lt = n2->getLeft();
    bool n2isLeft = false;
    if(n2p != NULL && (n2 == n2p->getLeft())) n2isLeft = true;


    Node<Key, Value>* temp;
    temp = n1->getParent();
    n1->setParent(n2->getParent());
    n2->setParent(temp);

    temp = n1->getLeft();
    n1->setLeft(n2->getLeft());
    n2->setLeft(temp);

    temp = n1->getRight();
    n1->setRight(n2->getRight());
    n2->setRight(temp);

    char temp2 = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(temp2);

    if( (n1r != NULL && n1r == n2) ) {
        n2->setRight(n1);
        n1->setParent(n2);
    }
    else if( n2r != NULL && n2r == n1) {
        n1->setRight(n2);
        n2->setParent(n1);

    }
    else if( n1lt != NULL && n1lt == n2) {
        n2->setLeft(n1);
        n1->setParent(n2);

    }
    else if( n2lt != NULL && n2lt == n1) {
        n1->setLeft(n2);
        n2->setParent(n1);

    }


    if(n1p != NULL && n1p != n2) {
        if(n1isLeft) n1p->setLeft(n2);
        else n1p->setRight(n2);
    }
    if(n1r != NULL && n1r != n2) {
        n1r->setParent(n2);
    }
    if(n1lt != NULL && n1lt != n2) {
        n1lt->setParent(n2);
    }

    if(n2p != NULL && n2p != n1) {
        if(n2isLeft) n2p->setLeft(n1);
        else n2p->setRight(n1);
    }
    if(n2r != NULL && n2r != n1) {
        n2r->setParent(n1);
    }
    if(n2lt != NULL && n2lt != n1) {
        n2lt->setParent(n1);
    }


    if(this->mRoot == n1) {
        this->mRoot = n2;
    }
    else if(this->mRoot == n2) {
        this->mRoot = n1;
    }

}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif