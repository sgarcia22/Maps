#ifndef HASHOPEN_H_INCLUDED
#define HASHOPEN_H_INCLUDED
#define maximum(a,b) (a > b ? a : b)
#include <math.h>
#define FIBONACCI_NUMBER 0.618033887
#define DEFAULT 127

#include <stdexcept>

namespace cop3530 {

template <typename key, typename value, size_t (*hash) (const key &),  bool (*equal) (const key &, const key &)>
//Hash Map with Open Addressing
class HASHOPEN {

private:

    KVPair<key,value> ** hash_table;
    size_t capacity_arr;

public:

///TODO: Big 5, Iterator

	HASHOPEN (size_t inputted_capacity = DEFAULT) {
		hash_table = new KVPair<key,value> * [inputted_capacity + 1] ();
		capacity_arr = inputted_capacity;
	}

	~HASHOPEN () {
        //Delete all the items
        for (int i = 0; i < capacity (); ++i) {
            if (hash_table[i]) {
                delete hash_table[i];
            }
        }
        delete [] hash_table;
	}


	//Deep Copy
    HASHOPEN(const HASHOPEN &orig) : hash_table (nullptr),  capacity_arr (0) {
        if (!hash_table)
            return;
        hash_table = new KVPair<key,value> * [capacity_arr] ();
        capacity_arr = orig.capacity_arr;
        for (int i = 0; i < capacity_arr; ++i) {
            hash_table[i] = orig.hash_table[i];
        }
    }

    //Deep Copy Assignment Operator
    HASHOPEN & operator=(const HASHOPEN & rhs) {
        if (hash_table)
            this->~HASHOPEN;
        using std::swap;
        swap (rhs.hash_table, hash_table);
        swap (rhs.capacity_arr, capacity_arr);
        rhs.hash_table = nullptr;
        rhs.capacity_arr = 0;
        return * this;
    }

    //Move Constructor
    HASHOPEN (HASHOPEN && s) noexcept : hash_table (s.hash_table),  capacity_arr (s.capacity_arr) {
        s.hash_table = nullptr;
        s.capacity_arr = 0;
    }

    //Move Assignment Operator
    HASHOPEN & operator=(HASHOPEN && rhs) noexcept {
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
        for (int i = 0; i < capacity () + 1 ; ++i) {
            if (hash_table[i]) {
                temp_size++;
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

            hash_table[curr_index] = new KVPair<key,value> (a, b);
        }
        else {
            size_t temp_key = 1;
            while (hash_table[curr_index]) {
                //Quadratic Probing
                curr_index = (curr_index + temp_key * temp_key) % (capacity() + 1) ;
                temp_key++;
            }
            hash_table[curr_index] = new KVPair<key,value> (a, b);
        }
	}

	void remove (key a) {
        if (!contains (a))
            throw std::runtime_error ("remove: hash table does not have that key.\n");
        size_t curr_index = hash(a);
        if (hash_table[curr_index] && equal(hash_table[curr_index]->get_key(), a)) {
            delete hash_table[curr_index];
            hash_table[curr_index] = nullptr;
        }
        else {
            size_t temp_probe = 1;
            while (true) {
                if (hash_table[curr_index])
                    if (equal(hash_table[curr_index]->get_key(), a)) {
                        delete hash_table[curr_index];
                        hash_table[curr_index] = nullptr;

                        return;
                    }
                //Quadratic Probing
                curr_index = (curr_index + temp_probe * temp_probe) % (capacity() + 1) ;
                temp_probe++;
            }
        }
	}
    ///TODO: FIX TO BY REFERENCE
	value & lookup (key a) {
	    if (!contains (a))
            throw std::runtime_error("lookup: value not found");
        size_t curr_index = hash(a);
        if (hash_table[curr_index] && hash_table[curr_index] && equal(hash_table[curr_index]->get_key(), a)) {
           // value temp = (hash_table[curr_index]->get_value());
           // value * temp2 = &temp;
           // return *temp2;
           return (hash_table[curr_index]->get_value());
        }
        else {
            size_t temp_probe = 1;
            while (true) {
                if (hash_table[curr_index] && equal(hash_table[curr_index]->get_key(), a)) {
                    /*value temp = hash_table[curr_index]->get_value();
                    value * temp2 = &temp;
                    return *temp2;*/
                    return (hash_table[curr_index]->get_value());
                }
                else {
                    //Quadratic Probing
                    curr_index = (curr_index + temp_probe * temp_probe) % (capacity() + 1) ;
                    temp_probe++;
                }
            }
        }
	}

	bool contains (key a) {
        size_t curr_index = hash(a);
        if (hash_table[curr_index] && equal(hash_table[curr_index]->get_key(), a))
            return true;
        /*else {
            size_t temp_probe = 1;

            while (hash_table[curr_index]) {
                if (equal(hash_table[curr_index]->get_key(), a))
                    return true;
                else {
                    //Quadratic Probing
                    curr_index = (curr_index + temp_probe * temp_probe) % (capacity() + 1) ;
                    temp_probe++;
                }
            } */
            //Using O (n) contains because removing a value beforehand will halt
            //the quadratic probing and make it seem like the key is not there when it is
            for (int i = 0; i < capacity () + 1; ++i) {
                if (hash_table[i] && equal(hash_table[i]->get_key(), a))
                    return true;
            }


        return false;
	}

	void clear () {
        //Delete all the items
        for (int i = 0; i < capacity () + 1; ++i) {
            if (hash_table[i]) {
                delete hash_table[i];
            }
        }
        delete [] hash_table;
        hash_table = new KVPair<key,value> * [capacity_arr + 1] ();
	}

	bool is_full () {
        return size() >= capacity();
	}

	bool is_empty () {
        return size() == 0;
	}

	void print ()
	{
	    for (int i = 0; i < capacity () + 1; ++i ) {
            if (hash_table[i])
                std::cout << hash_table[i]->get_key() << " ; ";
            else
                std::cout << " - ";
	    }
	    std::cout << "\n";
	}

    void test () {
        for (int i = 0; i  < capacity () + 1; ++i) {
            if (!hash_table[i])
                std::cout << "FALSEFALSEFALSE\n\n";
        }
    }

    //Iterator Implementation
    template <typename keyT, typename valueT>
    class HASHOPEN_Iter {
    public:
        //Iterator Compatibility
        using iterator_category = std::forward_iterator_tag;
        using iterator_category_const = const std::forward_iterator_tag;
        using value_type = KVPair<keyT, valueT>;
        using reference =  KVPair<keyT, valueT>&;
        using pointer =  KVPair<keyT, valueT>*;
        using difference_type = std::ptrdiff_t;

        //Type aliases
        using self_type = HASHOPEN_Iter;
        using self_reference = HASHOPEN_Iter&;

    private:
        pointer * curr;

    public:
        explicit HASHOPEN_Iter<keyT,valueT> (KVPair<key,value> ** start = nullptr) : curr(start) {}
       // HASHOPEN_Iter (const KVPair<keyT,valueT const> *& src) : curr (src.curr) {}

        //Return the key
        pointer  operator*() const {return (curr ? *curr : nullptr);}
        //Return the KVPair Object
        pointer operator->() const {return (curr ? curr : nullptr);}

        self_reference operator=(HASHOPEN_Iter<keyT,valueT> const & src) {
            if (this == src)
                return *this;
            curr = src.curr;
            return *this;
        }
        //Pre-increment operator overload
        self_reference operator++() {
        //    while (! (*     curr))
                ++curr;
            return *this;
        }
        //Post-increment operator overload
        self_reference operator++ (int) {
            self_type temp (*this);
          //  while (! (curr))
                ++curr;
            return temp;
        }
        //Boolean Overloaded Operators
        bool operator==(HASHOPEN_Iter<keyT, valueT> const& rhs) const {
            return (*curr) == (*rhs.curr);
        }
        bool operator!=(HASHOPEN_Iter<keyT, valueT> const& rhs) const {
			return (*curr) != (*rhs.curr);
			//(*curr)->get_key() != (*rhs.curr)->get_key();
        }
    };

    using key_type = key;
    using value_type = value;
    using iterator = HASHOPEN_Iter<key_type, value_type>;
    using const_iterator = HASHOPEN_Iter<key_type, value_type const>;

    //Iterator begin and end functions
    iterator begin () {return iterator(hash_table);}
    iterator end () {return iterator(hash_table + capacity() + 1);}

    const_iterator begin () const {return const_iterator(hash_table);}
    const_iterator end () const {return const_iterator(hash_table + capacity() + 1);}

};
}

///TODO
/*
-Return by reference
-Iterator
-Hashing for Strings
*/

#endif // HASHOPEN_H_INCLUDED
