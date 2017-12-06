#ifndef SSLL_H
#define SSLL_H

#include <iostream>
#include <typeinfo>
#include <utility>

namespace cop3530 {

template <typename element>

class SSLL
{
    public:

        struct Node {
            element data;
            Node * next;
            ~Node() {};
        };

        //Constructors/Destructors
        SSLL();
        SSLL(const SSLL&);
        SSLL& operator=(const SSLL&);
        SSLL(SSLL &&s) noexcept;
        SSLL& operator=(SSLL&&) noexcept;
        ~SSLL();


        void insert(element object, int position);
        void push_back (element object);
        void push_front (element object);
        void replace (element object, int position);
        element remove (int position);
        element pop_back ();
        element pop_front ();
        element & item_at (int position);
        element & peek_back ();
        element & peek_front ();
        bool is_empty ();
        bool is_full ();
        size_t length ();
        void clear ();
        bool contains (element object, bool (*equals_function) (const element&, const element&));
        std::ostream& print (std::ostream& out);
        element * contents();
        bool equals (element a, element b);

        private:

            Node * head;
            Node * tail;

        public:


        template <typename DataT>
        class SSLL_Iter {

            public:
                //Iterator Compatibility
                using iterator_category = std::forward_iterator_tag;
                using iterator_category_const = const std::forward_iterator_tag;
                using value_type = element;
                using reference = element&;
                using pointer = element*;
                using difference_type = std::ptrdiff_t;

                //Type aliases
                using self_type = SSLL_Iter;
                using self_reference = SSLL_Iter&;

            private:
                Node * here;

            public:
                //Points to here
                explicit SSLL_Iter(Node * start = nullptr) : here (start) {}
                SSLL_Iter (const SSLL_Iter& src) : here (src.here) {}

                reference operator*() const {return here->data;}
                reference operator->() const {return here;}
                //Equals sign operator
                self_reference operator=(SSLL_Iter<DataT> const& src) {
                    if (this == &src)
                        return *this;
                    here = src.here;
                    return *this;
                }
                //Pre-increment operator
                self_reference operator++() {
                    if (here != nullptr) {
                        here = here->next;
                        return *this;
                    }
                }
                //Post-increment operator
                self_type operator++(int) {
                    if (here != nullptr) {
                        self_type temp(*this);
                        here = here->next;
                        return temp;
                    }
                }

                bool operator==(SSLL_Iter<DataT> const& rhs) const {return here == rhs.here;}
                bool operator!=(SSLL_Iter<DataT> const& rhs) const {return here != rhs.here;}


        };

        //Iterator Implementation
        using value_type = element;
        using iterator = SSLL_Iter<element>;
        using const_iterator = SSLL_Iter<element const>;
        //Iterator begin and end functions
        iterator begin () {return iterator(head);}
        iterator end () {return iterator(tail->next);}

        const_iterator begin() const {return const_iterator(head);}
        const_iterator end() const {return const_iterator(tail->next);}
};

}

//Constructor, initialize Nodes
template <typename element>
cop3530::SSLL<element>::SSLL() {
    //Set head and tail equal to null
    head = tail = nullptr;
}

template <typename element>
//Deep Copy Constructor
cop3530::SSLL<element>::SSLL(const SSLL &orig) : head (nullptr) , tail (nullptr) {
    //If no list then exit the deep copy
    if (!orig.head)
        return;
    //Temporary variable for the head
    Node * temp = orig.head;
    //Allocate a new node
    head = new Node ();
    head->data = temp->data;
    head->next = nullptr;

    tail = head;
    temp = temp->next;
    //Copy orig's values into the new list
    while (temp) {
        tail->next = new Node ();
        tail = tail->next;
        //Copy the data
        tail->data = temp->data;
        tail->next = nullptr;
        //Keep going through orig's SSLL
        temp = temp->next;
    }
}

template <typename element>
//Copy Assignment Operator
cop3530::SSLL<element>& cop3530::SSLL<element>::operator=(const SSLL &rhs) {
    if (head)
        this->~SSLL();
    //Swap the old values to the new values, deleting the old after
    using std::swap;
    swap(rhs.head, head);
    swap(rhs.tail, tail);
    rhs.head = rhs.tail = nullptr;
    return *this;
}

template <typename element>
//Move Constructor
cop3530::SSLL<element>::SSLL(SSLL &&s) noexcept : head(s.head), tail(s.tail) {
    s.head = s.tail = nullptr;
}

template <typename element>
//Move Assignment Operator
cop3530::SSLL<element>& cop3530::SSLL<element>::operator=(SSLL &&rhs) noexcept {
    if (this != &rhs) {
        delete head;
        delete tail;

        using std::swap;
        swap(rhs.head, head);
        swap(rhs.tail, tail);

        rhs.head = rhs.tail = nullptr;
    }
}

template <typename element>
//Class Destructor
cop3530::SSLL<element>::~SSLL() {
    Node * temp = head;
    while (temp) {
        Node * next = temp->next;
        delete temp;
        temp = next;
    }
    head = tail = nullptr;
}

template <typename element>
//Pushes an element to the front
void cop3530::SSLL<element>::push_front(element object) {
    if (!is_full()) {
        if (is_empty()) {
            head = new Node();
            head->data = object;
            head->next = tail;
            tail = head;
            tail->next = nullptr;
        }
        else {
            Node * temp = new Node();
            temp->data = object;
            temp->next = head;
            head = temp;
            temp = nullptr;
            delete temp;
        }
    }
    else
        throw std::runtime_error("The List is full, cannot push to the front.\n ");
}

template <typename element>
//Pushes an element to the back
void cop3530::SSLL<element>::push_back(element object) {

    if (!is_full()) {
        Node * curr = new Node();
        curr->data = object;
        curr->next = nullptr;
        //If there is no current head pointer
        if (!head) {
            //Since there are no more elements the head and tail pointers equal the same thing
            head = curr;
            tail = curr;
        }
        else {
            //Make the next element in the list equal the current Node
            tail->next = curr;
            tail = tail->next;
        }
        curr = nullptr;
        delete curr;
    }
    else
        throw std::runtime_error("The List is full, cannot push back.\n ");

}

template <typename element>
//Inserts an element at the specified position
void cop3530::SSLL<element>::insert(element object, int position) {
    if (is_full())
        throw std::runtime_error("The list is full, cannot insert.\n ");
    if (is_empty() && signed(position)  > 0)
        throw std::runtime_error("The list is empty, cannot insert at desired position.\n ");
    if (position > length() || signed(position)  < 0)
        throw std::runtime_error("Invalid Index; no element at the specified position.\n ");
    if (position == 0) {
        push_front(object);
        return;
    }
    if (position == length()) {
        push_back(object);
        return;
    }

    Node * temp = head;
    int index = 0;
    while (temp) {
        if (index + 1 == position) {
            Node * inserted_node = new Node();
            inserted_node->data = object;
            inserted_node->next = temp->next;
            temp->next = inserted_node;
            break;
        }
        temp = temp->next;
        ++index;
    }
    temp = nullptr;
    delete temp;
}

template <typename element>
//Replaces an element at the specified position
void cop3530::SSLL<element>::replace(element object, int position) {
    if (is_empty())
        throw std::runtime_error("The list is empty, cannot replace.\n ");
    if (position >= length() || signed(position)  < 0)
        throw std::runtime_error("Invalid Index; no element at the specified position.\n ");
    Node * temp = head;
    int index = 0;
    while (temp) {
        if (index == position) {
            temp->data = object;
            break;
        }
        temp = temp->next;
        ++index;
    }
}

template <typename element>
//Removes an element at the specified position
element cop3530::SSLL<element>::remove(int position) {
    if (is_empty())
        throw std::runtime_error("The list is empty, cannot replace.\n ");
    if (position >= length() || signed(position)  < 0)
        throw std::runtime_error("Invalid Index; no element at the specified position.\n ");
    if (position == 0) {
        element to_return = pop_front();
        return to_return;
    }
    if (position == length() - 1) {
        element to_return = pop_back();
        return to_return;
    }
    Node * temp = head;
    int index = 0;
    element to_return;
    while (temp) {
        if (index == position - 1) {
			to_return = temp->next->data;
			Node * to_destroy = temp->next;
            temp->next = temp->next->next;
            //Make sure to delete the node
            delete to_destroy;
            break;
        }
        temp = temp->next;
        ++index;
    }
    return to_return;
}

template <typename element>
//Returns the item at a specified position
element & cop3530::SSLL<element>::item_at(int position) {
    if (position >= length() || signed(position)  < 0)
        throw std::runtime_error("Invalid Index; no element at the specified position.\n ");

    Node * temp = head;
    int index = 0;
    while (temp) {
        if (index == position) {
            return temp->data;
        }
        temp = temp->next;
        ++index;
    }
}

template <typename element>
//Removes and returns the last element
element cop3530::SSLL<element>::pop_back() {
    if (is_empty())
        throw std::runtime_error("The list is empty, cannot pop at back.\n ");

    element temp = tail->data;
    if (head == tail)
        clear();
    else {
        Node * curr = head;
        while (curr->next != tail) {
            curr = curr->next;
        }
        tail = curr;
        Node * to_destroy = tail->next;
        tail->next = nullptr;

        delete to_destroy;
    }
    return temp;
}

template <typename element>
//Returns the last element
element & cop3530::SSLL<element>::peek_back() {
    if (is_empty())
        throw std::runtime_error("The list is empty, cannot peek at back.\n ");
    return tail->data;
}

template <typename element>
//Removes and returns the first element
element cop3530::SSLL<element>::pop_front() {
    if (is_empty())
        throw std::runtime_error ("The list is empty, cannot pop at front.\n ");
    element frontItem = head->data;
    if (head == tail){
        clear();
        return frontItem;
    }
    Node * temp = head;
    head = temp->next;
	delete temp;

    return frontItem;
}

template <typename element>
//Returns the first element
element & cop3530::SSLL<element>::peek_front() {
    if (is_empty())
        throw std::runtime_error("The list is empty, cannot peek at front.\n ");
    return head->data;
}

template <typename element>
//Returns if the List is full
bool cop3530::SSLL<element>::is_full() {
    return false;
}

template <typename element>
//Returns if the List is empty
bool cop3530::SSLL<element>::is_empty() {
    return (head == NULL && tail == NULL ? true : false);
}

template <typename element>
//Returns the length of the current lsit
size_t cop3530::SSLL<element>::length() {
    if (!is_empty()) {
        size_t len = 0;
        Node * temp = head;
        while (temp) {
            ++len;
            temp = temp->next;
        }
        return len;
    }
    else
        return 0;

}

template <typename element>
//Clears the list
void cop3530::SSLL<element>::clear() {
    Node * temp = head;
    while (temp) {
        Node * next = temp->next;
        delete temp;
        temp = next;
    }
    head = tail = nullptr;
}

template <typename element>
//Prints all of the elements
std::ostream& cop3530::SSLL<element>::print (std::ostream& out) {
    if (is_empty())
        out << "<empty list>";
    else {
        out << "<";
        Node * temp = head;
        while (temp) {
            if (!temp->next) {
                out << temp->data;
            }
            else {
                out << temp->data << ",";
            }
            temp = temp->next;
        }
        out << ">";
    }
    return out;
}

template <typename element>
//Returns the contents of the list
element * cop3530::SSLL<element>::contents() {
    if (is_empty())
        throw std::runtime_error("The list is empty, there are no elements.\n ");
    element * values = new element[length()];
    Node * temp = head;
    int index = 0;
    while (temp) {
        values[index] = temp->data;
        temp = temp->next;
        ++index;
    }
    return values;
}

template <typename element>
//See if the list contains a certain element through a function pointer to make sure it is the exact element
bool cop3530::SSLL<element>::contains (element object, bool (*equals_function) (const element&, const element&))  {
    if (is_empty())
        return false;

    Node * temp = head;
    while (temp) {
        if (equals_function(object, temp->data))
            return true;
        temp = temp->next;
    }
    return false;
}
#endif // SSLL_H
