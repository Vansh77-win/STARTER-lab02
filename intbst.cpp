// intbst.cpp
// Implements class IntBST
// Vansh Srivastava 10/21

#include "intbst.h"

#include <iostream>
using std::cout;

// constructor sets up empty tree
IntBST::IntBST() { 
    root = nullptr;
}

// destructor deletes all nodes
IntBST::~IntBST() {
    clear(root);
}

// recursive helper for destructor
void IntBST::clear(Node *n) {
    if (n) {
        clear(n->left);
        clear(n->right);
        delete n;
    }
}

// insert value in tree; return false if duplicate
bool IntBST::insert(int value) {
    if (!root) {
        root = new Node(value);
        return true;
    }
    return insert(value, root);
}

// recursive helper for insert (assumes n is never 0)
bool IntBST::insert(int value, Node *n) {
    if (value < n->info) {

        if (!n->left) {

            n->left = new Node(value);
            return true;
        }
        return insert(value, n->left);

    } else if (value > n->info) {
        if (!n->right) {
            n->right = new Node(value);
            return true;
        }
        return insert(value, n->right);

    } else {
        return false; // duplicate value
    }
}

// print tree data pre-order
void IntBST::printPreOrder() const {

    printPreOrder(root);
    cout << endl;
}

// recursive helper for printPreOrder()
void IntBST::printPreOrder(Node *n) const {

    if (n) {
        cout << n->info << " ";
        printPreOrder(n->left);
        printPreOrder(n->right);
    }
}

// print tree data in-order, with helper
void IntBST::printInOrder() const {
    printInOrder(root);
}

void IntBST::printInOrder(Node *n) const {
    if (n) {
        printInOrder(n->left);
        cout << n->info << " ";
        printInOrder(n->right);
    }
}

// prints tree data post-order, with helper
void IntBST::printPostOrder() const {
    printPostOrder(root);
}

void IntBST::printPostOrder(Node *n) const {
    if (n) {
        printPostOrder(n->left);
        printPostOrder(n->right);
        cout << n->info << " ";
    }
}

// return sum of values in tree
int IntBST::sum() const {
    return sum(root);
}

// recursive helper for sum
int IntBST::sum(Node *n) const {
    if (!n) return 0;
    return n->info + sum(n->left) + sum(n->right);
}

// return count of values
int IntBST::count() const {
    return count(root);
}

// recursive helper for count
int IntBST::count(Node *n) const {
    if (!n) return 0;
    return 1 + count(n->left) + count(n->right);
}

// IMPLEMENT THIS FIRST: returns the node for a given value or NULL if none exists
// Parameters:
// int value: the value to be found
// Node* n: the node to start with (for a recursive call)
// Whenever you call this method from somewhere else, pass it
// the root node as "n"
IntBST::Node* IntBST::getNodeFor(int value, Node* n) const{
    if (!n) return NULL;
    if (value == n->info) return n;
    if (value < n->info) return getNodeFor(value, n->left);
    else return getNodeFor(value, n->right);
}

// returns true if value is in the tree; false if not
bool IntBST::contains(int value) const {
    return getNodeFor(value, root) != NULL;
}

// returns the Node containing the predecessor of the given value
IntBST::Node* IntBST::getPredecessorNode(int value) const{
    Node* n = root;
    Node* pred = nullptr;

    while (n && n->info != value) {
        if (value < n->info) {
            n = n->left;
        } else {
            pred = n;
            n = n->right;
        }
    }

    if (!n) return nullptr;

    if (n->left) {
        pred = n->left;
        while (pred->right) pred = pred->right;
    }

    return pred;
}

// returns the predecessor value of the given value or 0 if there is none
int IntBST::getPredecessor(int value) const{
    Node* pred = getPredecessorNode(value);
    return pred != nullptr ? pred->info : 0;
}

// returns the Node containing the successor of the given value
IntBST::Node* IntBST::getSuccessorNode(int value) const {
    Node* n = getNodeFor(value, root);
    if (!n) return NULL;

    // Case 1: Node has a right subtree. 
    if (n->right) {
        n = n->right;
        while (n->left) n = n->left;
        return n;
    }

    // Case 2: Node does NOT have a right subtree.
    Node* successor = NULL;
    Node* current = root;
    
    while (current != n) {
        if (n->info < current->info) {
            successor = current; 
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    return successor;
}


// returns the successor value of the given value or 0 if there is none
int IntBST::getSuccessor(int value) const{
    Node* succ = getSuccessorNode(value);
    return succ != nullptr ? succ->info : 0;
}

// deletes the Node containing the given value from the tree
// returns true if the node exist and was deleted or false if the node does not exist
bool IntBST::remove(int value) {
    Node* n = getNodeFor(value, root);
    if (!n) return false;

    // Case 3: Two children
    if (n->left && n->right) {
        Node* pred = getPredecessorNode(value);
        int predValue = pred->info;
        
        // FIX 1: Remove the predecessor node FIRST while its value is still unique 
        // in the tree. This prevents the infinite recursion.
        remove(predValue); 
        
        // Now safely update the current node's value
        n->info = predValue;
        return true;
    }

    // Case 1 & 2: One or zero children
    Node* child = nullptr;
    if (n->left) {
        child = n->left;
    } else {
        child = n->right;
    }

    if (n == root) {
        root = child;
    } else {
        // FIX 2: Correctly traverse to find the actual parent of 'n'
        Node* parent = root;
        while (parent != nullptr && parent->left != n && parent->right != n) {
            if (n->info < parent->info) {
                parent = parent->left;
            } else {
                parent = parent->right;
            }
        }

        // Reconnect the parent to the child
        if (parent != nullptr) {
            if (parent->left == n) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        }
    }
    
    delete n;
    return true;
}
