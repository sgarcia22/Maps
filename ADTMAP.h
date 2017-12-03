#ifndef ADTMAP_H_INCLUDED
#define ADTMAP_H_INCLUDED
#define max(a,b) (a > b ? a : b)

#include <iostream>
#include <stdexcept>
#include <stdlib.h> //rand()

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

private:

    node<key,value> * root;
    COMPARE comp;
    EQUALITY equal;

public:

    //Constructor
    BSTROOT () {root = nullptr;}

    //Rotate Right
    void ll (node<key,value> *& a) {
        node<key,value> * original = a;
        a = a->left;
        original->left = a->right;
        a->right = original;
    }

    //Rotate Left
    void rr (node<key,value> *& a) {
        node<key,value> * original = a;
        a = a->right;
        original->right = (a->left ? a->left : nullptr);
        a->left = original;
    }

    //Insert at the root
    void insert (key a, value b) {
        if (!root) {
            root = new node<key,value> ();
            root->priority = a;
            root->data = b;
            return;
        }
        //Call a recursive function
        insert_at_root(root, a, b);

    }

    node<key,value> * insert_at_root(node<key,value> *& curr, key a, value b) {
        if (!curr) {
                 std::cout << "entering\n";
            curr = new node<key,value> ();
            curr->priority = a;
            curr->data = b;
        }
        else if (comp(curr->priority, a)) {
            insert_at_root(curr->left, a, b);
            ll (curr);
        }
        else {
            std::cout << "entering\n";
            insert_at_root(curr->right, a, b);
            rr (curr);
        }
        return curr;
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
class BSTRAND {

private:

    node<key,value> * root;
    COMPARE comp;
    EQUALITY equal;

public:

    //Constructor
    BSTRAND () {root = nullptr;}

    //Recursive function to return the size of the tree
    size_t size() {
        return call_size (root);
    }

    size_t call_size (node<key,value> * a) {
        if (!a)
            return 0;
        return call_size (a->left) + call_size(a->right) + 1;
    }

    void insert (key a, value b) {
        //Get the 1 / N + 1 random number
        size_t random = (rand() % (size() + 1)) < 1;
        std::cout << random << std::endl;
        if (!random)
            insert_at_root(root, a, b);
        else
            insert_at_leaf(a, b);
    }

    //Rotate Right
    void ll (node<key,value> *& a) {
        node<key,value> * original = a;
        a = a->left;
        original->left = a->right;
        a->right = original;
    }

    //Rotate Left
    void rr (node<key,value> *& a) {
        node<key,value> * original = a;
        a = a->right;
        original->right = (a->left ? a->left : nullptr);
        a->left = original;
    }

    node<key,value> * insert_at_root(node<key,value> *& curr, key a, value b) {
        if (!curr) {
            curr = new node<key,value> ();
            curr->priority = a;
            curr->data = b;
        }
        else if (comp(curr->priority, a)) {
            insert_at_root(curr->left, a, b);
            ll (curr);
        }
        else {
            insert_at_root(curr->right, a, b);
            rr (curr);
        }
        return curr;
    }

    void insert_at_leaf (key a, value b) {
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
class AVL {

private:

    node<key,value> * root;
    COMPARE comp;
    EQUALITY equal;

public:

    //Constructor
    AVL () {root = nullptr;}

    //Rotate Right
    void ll (node<key,value> *& a) {
        node<key,value> * original = a;
        a = a->left;
        original->left = a->right;
        a->right = original;
    }

    //Rotate Left
    void rr (node<key,value> *& a) {
        node<key,value> * original = a;
        a = a->right;
        original->right = (a->left ? a->left : nullptr);
        a->left = original;
    }

    //Rotate right then left
    void rl (node<key,value> *& a) {
        ll_rotation(a->right);
        rr_rotation(a);
    }

    //Rotate left then right
    void (node<key,value> *& a) {
        rr_rotation(a->left);
        ll_rotation(a);
    }

    //Height of the tree
    size_t height () {
        return calculate_height (root);
    }
    //Return the height of the tree
    size_t calculate_height (node<key,value> *& a) {
        if (!a)
            return 0;
        return max(calculate_height(a->left), calculate_height(a->right)) + 1;
    }
    //Balance factor of the tree
    size_t balance () {
        return balance_factor(root);
    }

    size_t balance_factor (node<key,value> *& a) {
        return (height(a->left) - height(a->right));
    }

    //Balance the tree on every insert
    void fixer_upper (node<key,value> *& a) {
        size_t balance_number = balance();
        //The tree is balanced or roughly balanced
        if (!balance_number || balance_number == 1 || balance_number == -1)
            return;

    }

    void insert (key a, value b) {
        if (!root) {
            root = new node<key,value> ();
            root->priority = a;
            root->data = b;
            return;
        }
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
            fixer_upper (temp);
        }
        else {
            //Insert the new node to the right
            temp->right = new node<key,value> ();
            temp->right->priority = a;
            temp->right->data = b;
            fixer_upper (temp);
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
}

///TODO::
    ///Finish all implementations
    ///The Big 5
    ///Sunday: Part 1 Extra Functions, Part 2
    ///Monday: Part 3 & Write Test Cases CATCH
    ///Tuesday: Part 4 & Write Test Cases CATCH
    ///Wednesday: Finishing Touches

#endif // ADTMAP_H_INCLUDED
