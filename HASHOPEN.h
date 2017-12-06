#ifndef HASHOPEN_H_INCLUDED
#define HASHOPEN_H_INCLUDED
#define maximum(a,b) (a > b ? a : b)
#include <math.h>
#DEFINE FIBONACCI_NUMBER 0.618033887
#DEFINE DEFAULT 127

template <typename key>
struct equality  { bool operator()(key a, key b) {return a == b;} };

template <typename key, typename value>
//Fibonacci Hash Function
struct hash_func {
	size_t size = cop3530::HASHOPEN<key, value>::size();
	unsigned int operator() (key a) { return floor(size * (a * FIBONACCI_NUMBER - floor(a * FIBONACCI_NUMBER)))};
};

namespace cop3530 {

template <typename key, typename value, typename HASH = hash_func<key>, typename EQUALITY = equality<key>>
//Hash Map with Open Addressing
class HASHOPEN {

private:

    HASH hash;
    EQUALITY equal;
    value * hash_table;
    size_t capacity_arr;

public:

///TODO: Big 5, Iterator

	HASHOPEN (size_t inputted_capacity = DEFAULT) {
		hash_table = new value [inputted_capacity];
		capacity_arr = inputted_capacity;
	}

	size_t capacity () {
		return capacity_arr;
	}

	size_t size () {

	}

	void insert (key a, value b) {

	}

	void remove (key a) {


	}

	value & lookup (key a) {


	}

	bool contains (key a) {

	}

	void clear () {

	}

	bool is_full () {

	}

	//Implement Iterator

};
}
#endif // HASHOPEN_H_INCLUDED
