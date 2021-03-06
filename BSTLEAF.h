#ifndef BSTLEAF_INCLUDED
#define BSTLEAF_INCLUDED

#define maximum(a,b) (a > b ? a : b)

#include "node.h"
#include "KVPair.h"

#include <iostream>
#include <stdexcept>
#include <stdlib.h> //rand()
#include "SSLL.h"

namespace cop3530 {

template <typename key, typename value, bool (*comp) (const key &, const key &),  bool (*equal) (const key &, const key &)>
//Insert at the leaf
class BSTLEAF {
private:

    node<key,value> * root;

public:

    //Constructor
    BSTLEAF () {root = nullptr;}
    //Destructor
    ~BSTLEAF () {
        remove_all (root);
    }

    //Deep Copy Constructor
    BSTLEAF(const BSTLEAF & orig) : root (nullptr) {
        //If no root, then exit
        if (!orig.root)
            return;

        root = orig.root;
        root->priority = orig.root->priority;
        root->data = orig.root->priority;

        if (orig.root->left) {
            node<key,value> * temp = orig.root->left;
            root->left = make_copy (temp);
        }
        if (orig.root->right) {
            node<key,value> * temp = orig.root->right;
            root->right = make_copy (temp);
        }
    }

    node<key,value> * make_copy (node<key,value> * a) {
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

    void recursive_insert (node<key,value> * curr, key a, value b) {
        if (!curr) {
            curr = new node<key,value> ();
            curr->priority = a;
            curr->data = b;
            return;
        }
        if (comp(curr->priority, a)) {
            recursive_insert(curr->left, a, b);
        }
        else {
            recursive_insert(curr->right, a, b);
        }

    }

    value & lookup (key a) {
        if (!root)
            throw std::runtime_error ("lookup: there are no elements in the map\n");
        if (equal(root->priority,a))
            return root->data;
        return recursive_lookup (root, a);
        /*//Create a temporary node to traverse the list
        node<key,value> * curr = root;

        //While there exists node and the node is not equal to the key
        while ((curr->right || curr->left) && !equal(curr->priority, a)) {
           // std::cout << " KEY : " << a << " TESTING " << curr->priority << "\n";
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        }
        if (equal(curr->priority, a))
            return curr->data;
        else
            throw std::runtime_error ("lookup: the key does not equate to a value\n");
        */
    }

    value & recursive_lookup (node<key,value> * curr, key a) {
        if (!curr)
            throw std::runtime_error ("lookup: the key does not equate to a value\n");
        if (equal (curr->priority, a)) {
            return curr->data;
        }
        if (comp(curr->priority, a)) {
            recursive_lookup(curr->left, a);
        }
        else {
            recursive_lookup(curr->right, a);
        }
    }

    void remove (key a) {
        if (!root)
            throw std::runtime_error ("remove: there are no elements in the map\n");
        if (!contains(a))
            throw std::runtime_error ("remove: that element is not in the map\n");
        //Create a temporary node to traverse the list
        node<key,value> * curr = root;
        node<key,value> * temp = curr;
        //While there exists node and the node is not equal to the key
        while (!equal(curr->priority, a) && curr) {
            temp = curr;
            curr = (comp(curr->priority, a) ? curr->left : curr->right);
        }
        if (!equal(curr->priority, a))
            throw std::runtime_error ("remove: the key does not equate to a value\n");
        //First Case: The node has no children
        //Just delete directly
        if (!curr->left && !curr->right) {
            if (temp->right == curr) {
                temp->right = nullptr;
            }
            else {
                temp->left = nullptr;
            }
            delete curr;
        }
        //Second Case: The node has one child
        //Replace the current node with it's child
        else if ((curr->left || curr->right) && !(curr->left && curr->right)) {
            node<key,value> * temp_curr = (curr->left ? curr->left : curr->right);
            curr->priority = temp_curr->priority;
            curr->data = temp_curr->data;
            curr->left = temp_curr->left;
            curr->right = temp_curr->right;
            delete temp_curr;
        }
        //Third Case: The node has two children
        //Find the in-order successor and swap positions, later deleting the needed node

        else {
            node<key,value> * min = in_order_successor(curr->right);
            //Swap the data of the nodes
            curr->data = min->data;
            curr->priority = min->priority;
            node<key,value> * parent;
            if (curr->right == min)
                parent = curr;
            else if (curr->right->left == min)
                parent = curr->right;
            else
                parent = in_order_successor_second(curr->right);
            remove_node (min, parent);
        }
    }

    //Find the in-order successor of a node, or the minimum of the right sub-tree
    node<key,value> * in_order_successor (node<key,value> * a) {
        if (!a->left)
            return a;
        in_order_successor(a->left);
    }
     //Find the in-order successor of a node, or the minimum of the right sub-tree
    node<key,value> * in_order_successor_second (node<key,value> * a) {
        if (!a->left->left)
            return a;
        in_order_successor_second(a->left);
    }

    void remove_node (node<key,value> * curr, node<key,value> * parent) {
            if (!curr)
                return;
            if (!curr->left && !curr->right) {
                if (parent->right == curr) {
                    parent->right = nullptr;
                }
                else {
                    parent->left = nullptr;
                }
                delete curr;
            }
            //Second Case: The node has one child
            //Replace the current node with it's child
            else if ((curr->left || curr->right) && !(curr->left && curr->right)) {
                node<key,value> * temp_curr = (curr->left ? curr->left : curr->right);
                curr->priority = temp_curr->priority;
                curr->data = temp_curr->data;
                curr->left = temp_curr->left;
                curr->right = temp_curr->right;
                delete temp_curr;
            }
            //Third Case: The node has two children
            //Find the in-order successor and swap positions, later deleting the needed node
            else {
                node<key,value> * min = in_order_successor(curr->right);
                //Swap the data of the nodes
                curr->data = min->data;
                curr->priority = min->priority;
                if (curr->right == min)
                    parent = curr;
                else if (curr->right->left == min)
                    parent = curr->right;
                else
                    parent = in_order_successor_second(curr->right);
                remove_node (min, parent);
            }
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
        while ((curr->right || curr->left) && !equal(curr->priority, a)) {
            if (comp(curr->priority, a))
                if (curr->left) curr = curr->left;
                else break;
            else
                if (curr->right) curr = curr->right;
                else break;
        }
        if (curr && equal(curr->priority, a))
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
        root = nullptr;
    }

    void remove_all (node<key,value> *& a) {
       if (a) {
            if (a->left) remove_all(a->left);
            if (a->right) remove_all(a->right);
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
        return maximum(recursive_height(a->left), recursive_height(a->right)) + 1;
    }

    //Returns the tree's balance factor
    signed int balance () {
        return recursive_height(root->left) - recursive_height(root->right);
    }

     void in_order_traversal_iter (node<key,value> * a) {
        if (a) {
            in_order_traversal_iter(a->left);
            std::cout << a->priority << "  ";
            in_order_traversal_iter(a->right);
        }
    }

    //Iterator Implementation using In-Order Traversal
    //Iterator Implementation using In-Order Traversal
    ///Const still does not work
public:
    template<typename keyT, typename valueT>
    class BSTLEAF_Iter {
    public:
        using iterator_category = std::forward_iterator_tag;
        using iterator_category_const = const std::forward_iterator_tag;
        using value_type = KVPair<keyT, valueT>;
        using self_type = BSTLEAF_Iter<keyT,valueT>;
        using self_reference = BSTLEAF_Iter<keyT,valueT>& ;
        using pointer = KVPair<keyT, valueT>*;
        using node_pointer = cop3530::SSLL<pointer>*;
    private:
        node_pointer curr;

    public:
        explicit BSTLEAF_Iter<keyT,valueT> (cop3530::SSLL<KVPair<key,value>*> * start) : curr(start) {}
      //  BSTLEAF_Iter (const cop3530::SSLL<KVPair<key,value const>*> & src) : curr (src.curr) {}

        //Return the key
        pointer  operator*() const {return curr->peek_front();}
        //Return the KVPair Object
        pointer operator->() const {return curr->peek_front();}

        self_reference operator=(BSTLEAF_Iter const & src) {
            if (this == src)
                return *this;
            curr = src.curr;
            return *this;
        }
        //Pre-increment operator overload
        self_reference operator++() {
            curr->pop_front();
            return *this;
        }
        //Post-increment operator overload
        self_reference operator++ (int) {
            self_type temp (*this);
            curr->pop_front();
            return temp;
        }

        bool operator==(BSTLEAF_Iter<keyT, valueT> const& rhs) const {
            return curr->is_empty() == rhs.curr->is_empty();
        }
        bool operator!=(BSTLEAF_Iter<keyT, valueT> const& rhs) const {
			return curr->is_empty() != rhs.curr->is_empty();
		}
    };
    //Type aliases
    //using size_t = std::size_t; -> Compiler Complains
    using key_type = key;
    using value_type = value;
    using iterator = BSTLEAF_Iter<key_type, value_type>;
    using const_iterator = BSTLEAF_Iter<key_type, value_type const>;

    iterator begin () {
		cop3530::SSLL<KVPair<key,value>*> * KV_pair = new cop3530::SSLL<KVPair<key,value>*> ();
		in_order_traversal_iter_bstleaf(root, KV_pair);
		cop3530::SSLL<KVPair<key,value>*> * KV_pair_t = KV_pair;
		return iterator (KV_pair);
    }
    //When the list is empty the iterator has finished
    iterator end () {
        cop3530::SSLL<KVPair<key,value>*> * KV_pair = new cop3530::SSLL<KVPair<key,value>*> ();
		return iterator (KV_pair);
    }

    const_iterator begin () const {
        cop3530::SSLL<KVPair<key,value>*> * KV_pair = new cop3530::SSLL<KVPair<key,value>*> ();
		in_order_traversal_iter_bstleaf(root, KV_pair);
        const cop3530::SSLL<KVPair<key,value const>*> * KV_pair_const = KV_pair;
		return const_iterator (KV_pair_const);
    }

    const_iterator end () const {
        const cop3530::SSLL<KVPair<key,const value>*> * KV_pair = new cop3530::SSLL<KVPair<key,const value>*> ();
		return const_iterator (KV_pair);
    }

    void in_order_traversal_iter_bstleaf (node<key,value> * a, cop3530::SSLL<KVPair<key, value>*> * b) {
        if (a) {
            in_order_traversal_iter_bstleaf(a->left, b);
            //Make a new KV element
            KVPair<key,value> * temp = new KVPair<key,value> (a->priority, a->data);
            b->push_back (temp);
            temp = nullptr;
            delete temp;
            in_order_traversal_iter_bstleaf(a->right, b);
        }
    }
};
}
#endif
