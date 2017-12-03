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

template <typename key, typename value>
//Key-Value Pair Class
class KVPair {

public:

    KVPair (key a, value b) : class_key (a) {
        class_value = b;
    }

    key get_key () {
        return class_key;
    }

    key get_value () {
        return class_value;
    }

private:
    const key class_key;
    value class_value;
};

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
        remove_all (root);
    }
    ///TODO:: Test Constructors
    //Deep Copy Constructor
    BSTLEAF(const BSTLEAF & orig) : root (nullptr) {
        //If no root, then exit
        if (!orig.root)
            return;

        node<key,value> temp = orig.root;
        root = new node<key,value> ();
        root->priority = temp->priority;
        root->data = temp->priority;

        if (temp->left) {
            root->left = make_copy (temp->left);
        }
        if (temp->right) {
            root->right = make_copy (temp->right);
        }
    }

    node<key,value> * make_copy (const node<key,value> * a) {
        if (!a)
            return nullptr;
        node<key,value> * temp = new node<key,value>();
        temp->priority = a->priority;
        temp->data = a->data;
        temp->left = make_copy(a->left);
        temp->right = make_copy(a->right);
        return temp;
    }

    //Copy Assignment Operator
    BSTLEAF & operator=(const BSTLEAF &rhs) {
        if (root)
            this->~BSTLEAF();
        using std::swap;
        swap (rhs.root, root);
        rhs.root = nullptr;
        return * this;
    }

   //Move Constructor
    BSTLEAF (BSTLEAF && s) noexcept : root (s.root) {
        s.root = nullptr;
    }

    //Move Assignment Operator
    BSTLEAF & operator=(BSTLEAF && rhs) noexcept {
        if (this != &rhs) {
            delete root;

            using std::swap;
            swap (rhs.root, root);

            rhs.root = nullptr;
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

    //Additional Functions

    //Returns if the map contains a value associated with the inputted key
    bool contains (key a) {
        if (!root)
            return false;
        if (equal(root->priority,a))
            return true;
        //Create a temporary node to traverse the list
        node<key,value> * curr = root;
        //While there exists node and the node is not equal to the key
        while ((curr->right || curr->left) && !equal(curr->priority, a))
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        if (equal(curr->priority, a))
            return true;
        else
            return false;
    }

    //Returns if the tree is empty
    bool is_empty () {
        return (root ? false : true);
    }

    //Key-Value pairs can always be added to a tree
    bool is_full () {
        return false;
    }

    //Returns the number of key-value pairs in the map
    size_t size () {
        recursive_size(root);
    }

    size_t recursive_size (node<key,value> * a) {
        if (!a)
            return 0;
        return recursive_size(a->left) + recursive_size(a->right) + 1;
    }

    //Removes all of the nodes
    void clear () {
        remove_all (root);
    }

    void remove_all (node<key,value> *& a) {
       if (a) {
            //std::cout << a->priority << std::endl;
            remove_all(a->left);
            remove_all(a->right);
            a = nullptr;
            delete a;
       }
    }

    //Returns the height of the tree
    size_t height () {
        return recursive_height(root);
    }

    size_t recursive_height (node<key,value> * a) {
        if (!a)
            return 0;
        return max(recursive_height(a->left), recursive_height(a->right)) + 1;
    }

    //Returns the tree's balance factor
    signed int balance () {
        return recursive_height(root->left) - recursive_height(root->right);
    }

    //Iterator Implementation using In-Order Traversal

    ///FIX
public:
    template<typename keyT, typename valueT>
    class BSTLEAF_Iter {
    public:
        using iterator_category = std::forward_iterator_tag;
        using iterator_category_const = const std::forward_iterator_tag;
        using value_type = KVPair<keyT, valueT>;
        using self_type = BSTLEAF_Iter<keyT,valueT>;
        using self_reference = &BSTLEAF_Iter<keyT,valueT>;
    private:
        node<key,value> * here;
        value_type * arr;
    public:
        explicit BSTLEAF_Iter (node<key,value> * start = nullptr) : here (start) {
            in_order_traversal(here);
        }
        BSTLEAF_Iter (const BSTLEAF_Iter & src) : here (src.arr) {}

        value_type* in_order_traversal (node<key,value> * a) {
            if (!a)
                return arr;
            if (a == here)
                arr = new value_type [size()];
            in_order_successor(a->left);
            value_type temp = new value_type (root->priority, root->data);
            arr++ = temp;
            in_order_successor(a->right);
        }

        self_reference operator=(BSTLEAF_Iter const & src) {
            if (this == src)
                return *this;
            arr = src.arr;
            return *this;
        }
        //Pre-increment operator overload
        self_reference operator++() {
            ++arr;
            return *this;
        }
        //Post-increment operator overload
        self_reference operator++ (int) {
            self_type temp (*this);
            ++arr;
            return temp;
        }
        bool operator==(BSTLEAF_Iter<keyT, valueT> const& rhs) const {return arr = rhs.arr;}
        bool operator!=(BSTLEAF_Iter<keyT, valueT> const& rhs) const {return arr != rhs.arr;}

    };
    //Type aliases
    //using size_t = std::size_t;
    using key_type = key;
    using value_type = value;
    using iterator = BSTLEAF_Iter<key_type, value_type>;
    using const_iterator = BSTLEAF_Iter<key_type, value_type>;



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

    //Destructor
    ~BSTROOT () {remove_all(root);}

    //Deep Copy Constructor
    BSTROOT(const BSTROOT & orig) : root (nullptr) {
        //If no root, then exit
        if (!orig.root)
            return;

        node<key,value> temp = orig.root;
        root = new node<key,value> ();
        root->priority = temp->priority;
        root->data = temp->priority;

        if (temp->left) {
            root->left = make_copy (temp->left);
        }
        if (temp->right) {
            root->right = make_copy (temp->right);
        }
    }

    node<key,value> * make_copy (const node<key,value> * a) {
        if (!a)
            return nullptr;
        node<key,value> * temp = new node<key,value>();
        temp->priority = a->priority;
        temp->data = a->data;
        temp->left = make_copy(a->left);
        temp->right = make_copy(a->right);
        return temp;
    }

    //Copy Assignment Operator
    BSTROOT & operator=(const BSTROOT &rhs) {
        if (root)
            this->~BSTROOT();
        using std::swap;
        swap (rhs.root, root);
        rhs.root = nullptr;
        return * this;
    }

   //Move Constructor
    BSTROOT (BSTROOT && s) noexcept : root (s.root) {
        s.root = nullptr;
    }

    //Move Assignment Operator
    BSTROOT & operator=(BSTROOT && rhs) noexcept {
        if (this != &rhs) {
            delete root;

            using std::swap;
            swap (rhs.root, root);

            rhs.root = nullptr;
        }
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

    //Additional Functions

    //Returns if the map contains a value associated with the inputted key
    bool contains (key a) {
        if (!root)
            return false;
        if (equal(root->priority,a))
            return true;
        //Create a temporary node to traverse the list
        node<key,value> * curr = root;
        //While there exists node and the node is not equal to the key
        while ((curr->right || curr->left) && !equal(curr->priority, a))
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        if (equal(curr->priority, a))
            return true;
        else
            return false;
    }

    //Returns if the tree is empty
    bool is_empty () {
        return (root ? false : true);
    }

    //Key-Value pairs can always be added to a tree
    bool is_full () {
        return false;
    }

    //Returns the number of key-value pairs in the map
    size_t size () {
        recursive_size(root);
    }

    size_t recursive_size (node<key,value> * a) {
        if (!a)
            return 0;
        return recursive_size(a->left) + recursive_size(a->right) + 1;
    }

    //Removes all of the nodes
    void clear () {
        remove_all (root);
    }

    void remove_all (node<key,value> *& a) {
       if (a) {
            //std::cout << a->priority << std::endl;
            remove_all(a->left);
            remove_all(a->right);
            a = nullptr;
            delete a;
       }
    }

    //Returns the height of the tree
    size_t height () {
        return recursive_height(root);
    }

    size_t recursive_height (node<key,value> * a) {
        if (!a)
            return 0;
        return max(recursive_height(a->left), recursive_height(a->right)) + 1;
    }

    //Returns the tree's balance factor
    signed int balance () {
        return recursive_height(root->left) - recursive_height(root->right);
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

    //Destructor
    ~BSTRAND () {remove_all(root);}

    //Deep Copy Constructor
    BSTRAND(const BSTRAND & orig) : root (nullptr) {
        //If no root, then exit
        if (!orig.root)
            return;

        node<key,value> temp = orig.root;
        root = new node<key,value> ();
        root->priority = temp->priority;
        root->data = temp->priority;

        if (temp->left) {
            root->left = make_copy (temp->left);
        }
        if (temp->right) {
            root->right = make_copy (temp->right);
        }
    }

    node<key,value> * make_copy (const node<key,value> * a) {
        if (!a)
            return nullptr;
        node<key,value> * temp = new node<key,value>();
        temp->priority = a->priority;
        temp->data = a->data;
        temp->left = make_copy(a->left);
        temp->right = make_copy(a->right);
        return temp;
    }

    //Copy Assignment Operator
    BSTRAND & operator=(const BSTRAND &rhs) {
        if (root)
            this->~BSTRAND();
        using std::swap;
        swap (rhs.root, root);
        rhs.root = nullptr;
        return * this;
    }

   //Move Constructor
    BSTRAND (BSTRAND && s) noexcept : root (s.root) {
        s.root = nullptr;
    }

    //Move Assignment Operator
    BSTRAND & operator=(BSTRAND && rhs) noexcept {
        if (this != &rhs) {
            delete root;

            using std::swap;
            swap (rhs.root, root);

            rhs.root = nullptr;
        }
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

    //Additional Functions

    //Returns if the map contains a value associated with the inputted key
    bool contains (key a) {
        if (!root)
            return false;
        if (equal(root->priority,a))
            return true;
        //Create a temporary node to traverse the list
        node<key,value> * curr = root;
        //While there exists node and the node is not equal to the key
        while ((curr->right || curr->left) && !equal(curr->priority, a))
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        if (equal(curr->priority, a))
            return true;
        else
            return false;
    }

    //Returns if the tree is empty
    bool is_empty () {
        return (root ? false : true);
    }

    //Key-Value pairs can always be added to a tree
    bool is_full () {
        return false;
    }

    //Returns the number of key-value pairs in the map
    size_t size () {
        recursive_size(root);
    }

    size_t recursive_size (node<key,value> * a) {
        if (!a)
            return 0;
        return recursive_size(a->left) + recursive_size(a->right) + 1;
    }

    //Removes all of the nodes
    void clear () {
        remove_all (root);
    }

    void remove_all (node<key,value> *& a) {
       if (a) {
            //std::cout << a->priority << std::endl;
            remove_all(a->left);
            remove_all(a->right);
            a = nullptr;
            delete a;
       }
    }

    //Returns the height of the tree
    size_t height () {
        return recursive_height(root);
    }

    size_t recursive_height (node<key,value> * a) {
        if (!a)
            return 0;
        return max(recursive_height(a->left), recursive_height(a->right)) + 1;
    }

    //Returns the tree's balance factor
    signed int balance () {
        return recursive_height(root->left) - recursive_height(root->right);
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

    //Destructor
    ~AVL () {remove_all(root);}

    //Deep Copy Constructor
    AVL(const AVL & orig) : root (nullptr) {
        //If no root, then exit
        if (!orig.root)
            return;

        node<key,value> temp = orig.root;
        root = new node<key,value> ();
        root->priority = temp->priority;
        root->data = temp->priority;

        if (temp->left) {
            root->left = make_copy (temp->left);
        }
        if (temp->right) {
            root->right = make_copy (temp->right);
        }
    }

    node<key,value> * make_copy (const node<key,value> * a) {
        if (!a)
            return nullptr;
        node<key,value> * temp = new node<key,value>();
        temp->priority = a->priority;
        temp->data = a->data;
        temp->left = make_copy(a->left);
        temp->right = make_copy(a->right);
        return temp;
    }

    //Copy Assignment Operator
    AVL & operator=(const AVL &rhs) {
        if (root)
            this->~AVL();
        using std::swap;
        swap (rhs.root, root);
        rhs.root = nullptr;
        return * this;
    }

   //Move Constructor
    AVL (AVL && s) noexcept : root (s.root) {
        s.root = nullptr;
    }

    //Move Assignment Operator
    AVL & operator=(AVL && rhs) noexcept {
        if (this != &rhs) {
            delete root;

            using std::swap;
            swap (rhs.root, root);

            rhs.root = nullptr;
        }
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

    //Rotate right then left
    void rl (node<key,value> *& a) {
        ll(a->right);
        rr(a);
    }

    //Rotate left then right
    void lr (node<key,value> *& a) {
        rr(a->left);
        ll(a);
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
    signed int balance () {
        return balance_factor(root);
    }

    signed int balance_factor (node<key,value> *& a) {
        return (calculate_height(a->left) - calculate_height(a->right));
    }

    //Balance the tree on every insert
    void fixer_upper (node<key,value> *& a) {
        signed int balance_number = balance_factor(a);
        std::cout << "balance number: " << balance_number << "\n";
        //The tree is balanced or roughly balanced
        if (!balance_number || balance_number == 1 || balance_number == -1)
            return;
        //See which tree is bigger with node bounding checks to avoid segmentation faults
        //Left subtree is bigger
        if (balance_number > 1) {
            if (!a->left->left)
                lr(a);
            else if (!a->left->right)
                ll(a);
            else if (comp(a->left->left->priority, a->left->right->priority))
                ll(a);
            else
                lr(a);
        }
        //Right subtree is bigger
        else {
            if (!a->right->right)
                rl(a);
            else if (!a->right->left)
                rr(a);
            if (comp(a->right->right->priority, a->right->left->priority))
                rr(a);
            else
                rl(a);
        }
    }

    void insert (key a, value b) {
        if (!root) {
            root = new node<key,value> ();
            root->priority = a;
            root->data = b;
            return;
        }
        recursive_insert (root, a, b);
    }

    void recursive_insert (node<key,value> *& curr, key a, value b) {
        if (!curr) {
            curr = new node<key,value> ();
            curr->priority = a;
            curr->data = b;
            return;
        }
        //If the current node's priority is bigger
        if (comp(curr->priority, a)) {
            //Insert the new node to the left
            recursive_insert(curr->left, a, b);
            fixer_upper (curr);
        }
        else {
            //Insert the new node to the right
            recursive_insert(curr->right, a, b);
            fixer_upper (curr);
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

        //Additional Functions

    //Returns if the map contains a value associated with the inputted key
    bool contains (key a) {
        if (!root)
            return false;
        if (equal(root->priority,a))
            return true;
        //Create a temporary node to traverse the list
        node<key,value> * curr = root;
        //While there exists node and the node is not equal to the key
        while ((curr->right || curr->left) && !equal(curr->priority, a))
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        if (equal(curr->priority, a))
            return true;
        else
            return false;
    }

    //Returns if the tree is empty
    bool is_empty () {
        return (root ? false : true);
    }

    //Key-Value pairs can always be added to a tree
    bool is_full () {
        return false;
    }

    //Returns the number of key-value pairs in the map
    size_t size () {
        recursive_size(root);
    }

    size_t recursive_size (node<key,value> * a) {
        if (!a)
            return 0;
        return recursive_size(a->left) + recursive_size(a->right) + 1;
    }

    //Removes all of the nodes
    void clear () {
        remove_all (root);
    }

    void remove_all (node<key,value> *& a) {
       if (a) {
            //std::cout << a->priority << std::endl;
            remove_all(a->left);
            remove_all(a->right);
            a = nullptr;
            delete a;
       }
    }

};
}

///TODO::
    ///Today: Part 2
    ///Monday: Part 3 & Write Test Cases CATCH
    ///Tuesday: Part 4 & Write Test Cases CATCH
    ///Wednesday: Finishing Touches

#endif // ADTMAP_H_INCLUDED
