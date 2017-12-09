#ifndef HASHBUCKET_H_INCLUDED
#define HASHBUCKET_H_INCLUDED
#define maximum(a,b) (a > b ? a : b)
#include <math.h>
#define FIBONACCI_NUMBER 0.618033887
#define DEFAULT 127

#include <stdexcept>

template <typename key, typename value>
struct hash_node {
    key priority;
    value data;
    hash_node * next = nullptr;
};

namespace cop3530 {

template <typename key, typename value, size_t (*hash) (const key &),  bool (*equal) (const key &, const key &)>
//Hash Map with Open Addressing
class HASHBUCKET {

private:

public:

private:

    hash_node<key,value> ** hash_table;
    size_t capacity_arr;

public:

///TODO: Big 5, Iterator

	HASHBUCKET (size_t inputted_capacity = DEFAULT) {
		hash_table = new hash_node<key,value> * [inputted_capacity];
		capacity_arr = inputted_capacity;
		for (int i = 0; i < capacity_arr; ++i)
            hash_table[i] = nullptr;
	}

	~HASHBUCKET () {
        //Delete all the items
        for (int i = 0; i < capacity (); ++i) {
            if (hash_table[i]) {
                hash_node<key,value> * temp = hash_table[i];
                if (hash_table[i]->next) {
                    while (hash_table[i]) {
                        temp = hash_table[i];
                        hash_table[i] = hash_table[i]->next;
                        delete temp;
                    }
                }
                else {
                    delete hash_table[i];
                }
            }
        }
        delete [] hash_table;
	}

	//Deep Copy
    HASHBUCKET(const HASHBUCKET &orig) : hash_table (nullptr),  capacity_arr (0) {
        if (!hash_table)
            return;
        hash_table = new value [capacity_arr];
        capacity_arr = orig.capacity_arr;
        for (int i = 0; i < capacity_arr; ++i) {
            hash_table[i] = orig.hash_table[i];
        }
    }

    //Deep Copy Assignment Operator
    HASHBUCKET & operator=(const HASHBUCKET & rhs) {
        if (hash_table)
            this->~HASHBUCKET;
        using std::swap;
        swap (rhs.hash_table, hash_table);
        swap (rhs.capacity_arr, capacity_arr);
        rhs.hash_table = nullptr;
        rhs.capacity_arr = 0;
        return * this;
    }

    //Move Constructor
    HASHBUCKET (HASHBUCKET && s) noexcept : hash_table (s.hash_table),  capacity_arr (s.capacity_arr) {
        s.hash_table = nullptr;
        s.capacity_arr = 0;
    }

    //Move Assignment Operator
    HASHBUCKET & operator=(HASHBUCKET && rhs) noexcept {
        if (this != &rhs) {
            delete hash_table;


            using std::swap;
            swap (rhs.hash_table, hash_table);
            swap (rhs.capacity_arr, capacity_arr);

            rhs.hash_table = nullptr;
            rhs.capacity_arr = 0;
        }
    }

	size_t capacity () {
		return capacity_arr;
	}

	size_t size () {
        size_t temp_size = 0;
        for (int i = 0; i < capacity () ; ++i) {
            if (hash_table[i]) {
                ++temp_size;
                if (hash_table[i]->next) {
                    hash_node<key,value> * temp = hash_table[i];
                    temp = temp->next;
                    while (temp) {
                        ++temp_size;
                        temp = temp->next;
                    }
                }
            }
        }

        return temp_size;
	}

	//Return hash_map's load factor
	double load () {
        return (size() / capacity());
	}

	void insert (key a, value b) {
        if (is_full())
            throw std::runtime_error ("insert: hash table is full, cannot insert.\n");
        size_t curr_index = hash(a);
        if (!hash_table[curr_index]) {
            hash_table[curr_index] = new hash_node<key,value>();
            hash_table[curr_index]->data = b;
            hash_table[curr_index]->priority = a;
        }
        else {
            hash_node<key,value> * temp = hash_table[curr_index];
            while (temp) {
                if (!temp->next) {
                    temp->next = new hash_node<key,value>();
                    temp->next->data = b;
                    temp->next->priority = a;
                    temp = temp->next;
                    temp = nullptr;
                    delete temp;
                    break;
                }
                temp = temp->next;
            }
        }
	}

	void remove (key a) {
        if (!contains (a))
            throw std::runtime_error ("remove: value not found\n");
        size_t curr_index = hash(a);
        if (equal(hash_table[curr_index]->priority, a)) {
            hash_node<key,value> * temp = hash_table[curr_index];
            temp = temp->next;
            delete hash_table[curr_index];
            hash_table[curr_index] = nullptr;
            hash_table[curr_index] = temp;
        }
        else {

            hash_node<key,value> * temp = hash_table[curr_index];
            hash_node<key,value> * prev = temp;
            while (!equal(temp->priority, a)) {
                prev = temp;
                temp = temp->next;
            }
            prev->next = temp->next;
            temp = nullptr;
            delete temp;
        }
	}

	value & lookup (key a) {
	    if (!contains (a))
            throw std::runtime_error("lookup: key not found\n");
        size_t curr_index = hash(a);
        if (hash_table[curr_index]) {
            hash_node<key,value> * temp = hash_table[curr_index];
            while (!equal(temp->priority, a))
                temp = temp->next;
            return temp->data;
        }
        else
            throw std::runtime_error("lookup: key not found\n");
	}

	bool contains (key a) {
        size_t curr_index = hash(a);
        if (hash_table[curr_index]) {
            hash_node<key,value> * temp = hash_table[curr_index];
            while (temp) {
                if (equal (temp->priority, a))
                    return true;
                temp = temp->next;
            }
        }
        return false;
	}

	void clear () {
        //Delete all the items
        for (int i = 0; i < capacity (); ++i) {
            if (hash_table[i]) {
                hash_node<key,value> * temp = hash_table[i];
                if (hash_table[i]->next) {
                    while (hash_table[i]) {
                        temp = hash_table[i];
                        hash_table[i] = hash_table[i]->next;
                        delete temp;
                    }
                }
                else {
                    delete hash_table[i];
                }
            }
        }
        delete [] hash_table;
        hash_table = new hash_node<key,value> * [capacity()];
        for (int i = 0; i < capacity(); ++i)
            hash_table[i] = nullptr;
	}

	//Linked List is never full
	bool is_full () {
        return false;
	}

	bool is_empty () {
        return size() == 0;
	}

	void print ()
	{
	    for (int i = 0; i < capacity (); ++i ) {
            if (hash_table[i]) {
                hash_node<key,value> * temp = hash_table[i];
                while (temp) {
                    std::cout << temp->priority;
                    temp = temp->next;
                }
            }
	    }
	    std::cout << "\n";
	}
	///TODO: Implement Iterator





};
}

#endif
