#ifndef ADTMAP_H_INCLUDED
#define ADTMAP_H_INCLUDED

#include <iostream>
#include <stdexcept>

template <typename key, typename value>
struct node {
    key priority;
    value data;
    node * left = nullptr;
    node * right = nullptr;
};

template <typename key>
struct compare { bool operator()(key a, key b) {return (a > b ? true : false);} };

template <typename key>
struct equality  { bool operator()(key a, key b) {return a == b;} };

namespace cop3530 {

template <typename key, typename value, typename COMPARE = compare<key>, typename EQUALITY = equality<key>>
//Insert at the leaf
class BSTLEAF {

private:

    node<key,value> * root;
    COMPARE comp;
    EQUALITY equal;

public:

    //Constructor
    BSTLEAF () {root = nullptr;}
    //Destructor
    ~BSTLEAF () {
        remove_node (root);
    }

    ///Does not work -> Segmentation Fault
    void remove_node (node<key,value> * a) {
       if (a) {
            std::cout << a->priority << std::endl;
            remove_node(a->left);
            remove_node(a->right);
            a = nullptr;
            delete a;
       }
    }

    void insert (key a, value b) {
        if (!root) {
            root = new node<key,value> ();
            root->priority = a;
            root->data = b;
        }
        else {
            node<key,value> * curr = root;
            node<key,value> * temp = curr;
            //Go down the tree until a leaf is reached
            while (curr) {
                temp = curr;
                curr = (comp(curr->priority, a) ? curr->left : curr->right);
            }
            //If the current node's priority is bigger
            if (comp(temp->priority, a)) {
                //Insert the new node to the left
                temp->left = new node<key,value> ();
                temp->left->priority = a;
                temp->left->data = b;
            }
            else {
                //Insert the new node to the right
                temp->right = new node<key,value> ();
                temp->right->priority = a;
                temp->right->data = b;
            }
        }
    }

    key & lookup (key a) {
        if (!root)
            throw std::runtime_error ("lookup: there are no elements in the map\n");
        if (equal(root->priority,a))
            return root->data;
        //Create a temporary node to traverse the list
        node<key,value> * curr = root;
        //While there exists node and the node is not equal to the key
        while ((curr->right || curr->left) && !equal(curr->priority, a))
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        if (equal(curr->priority, a))
            return curr->data;
        else
            throw std::runtime_error ("lookup: the key does not equate to a value\n");
    }

    void remove (key a) {
        if (!root)
            throw std::runtime_error ("remove: there are no elements in the map\n");
        //Create a temporary node to traverse the list
        node<key,value> * curr = root;
        //While there exists node and the node is not equal to the key
        while (!equal(curr->priority, a) && curr)
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        if (!equal(curr->priority, a))
            throw std::runtime_error ("remove: the key does not equate to a value\n");
        //First Case: The node has no children
        //Just delete directly
        if (!curr->left && !curr->right)
            delete curr;
        //Second Case: The node has one child
        //Replace the current node with it's child
        else if ((curr->left || curr->right) && !(curr->left && curr->right)) {
            node<key,value> * temp = (curr->left ? curr->left : curr->right);
            std::cout << temp->priority << std::endl;
            if (curr == root)
                root = temp;
            else
                curr = temp;
            temp = nullptr;
            delete temp;
        }
        //Third Case: The node has two children
        //Find the in-order successor and swap positions, later deleting the needed node
        else {
            node<key,value> * min = in_order_successor(curr->right);
            //Swap the data of the nodes
            curr->data = min->data;
            curr->priority = min->priority;
            delete min;
        }
    }
    //Find the in-order successor of a node, or the minimum of the right sub-tree
    node<key,value> * in_order_successor (node<key,value> * a) {
        if (!a->left)
            return a;
        in_order_successor(a->left);
    }

};

template <typename key, typename value, typename COMPARE = compare<key>, typename EQUALITY = equality<key>>
//Insert at the root
class BSTROOT {



};

template <typename key, typename value, typename COMPARE = compare<key>, typename EQUALITY = equality<key>>
//Insert at the root
class BSTRAND {



};

template <typename key, typename value, typename COMPARE = compare<key>, typename EQUALITY = equality<key>>
//Insert at the root
class AVL {



};
}

///TODO::
    ///Finish all implementations
    ///The Big 5
    ///Add extra Functions -> Part 1
    ///Sunday: Part 2
    ///Monday: Part 3 & Write Test Cases CATCH
    ///Tuesday: Part 4 & Write Test Cases CATCH
    ///Wednesday: Finishing Touches

#endif // ADTMAP_H_INCLUDED
