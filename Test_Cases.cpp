//Include CATCH Framework
#define CATCH_CONFIG_MAIN

#include <iostream>
#include <stdexcept>
#include "catch.hpp"
//#include "ADTMAP.h"
#include "BSTLEAF.h"
#include "BSTROOT.h"
#include "BSTRAND.h"
#include "AVL.h"
#include "HASHOPEN.h"
#include "HASHBUCKET.h"
#define FIBONACCI_NUMBER 0.618033887

size_t hash_arr_size;

template <typename element>
bool compare (const element & a, const element & b) {
    return (a > b ? true : false);
}

template <typename element>
bool equality (const element & a, const element & b) {
    return (a == b);
}

template <typename key>
//Fibonacci Hash Function
size_t hash_func (const key & a) {
    return floor(hash_arr_size * (a * FIBONACCI_NUMBER - floor(a * FIBONACCI_NUMBER)));
}

TEST_CASE( "BSTLEAF" ) {

    cop3530::BSTLEAF<int,int, compare, equality> * bst = new cop3530::BSTLEAF<int,int, compare, equality> ();

    bst->insert(5,3);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 1);
    REQUIRE (bst->height() == 1);
    REQUIRE (!bst->balance());

    bst->insert(7, 65);
    bst->insert(4, 45);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 3);
    REQUIRE (bst->height() == 2);
    REQUIRE (!bst->balance());

    REQUIRE (bst->lookup(5) == 3);
    REQUIRE (bst->lookup(7) == 65);
    REQUIRE (bst->lookup(4) == 45);
    REQUIRE (!bst->contains(45));

    bst->insert(8, 100);
    bst->insert(1, 88);
    bst->insert(30, 1);
    bst->insert(28, 5);
    bst->insert(6, 6);
    bst->insert(3, 16);

    REQUIRE (bst->lookup(8) == 100);
    REQUIRE (bst->lookup(1) == 88);
    REQUIRE (bst->lookup(30) == 1);
    REQUIRE (bst->lookup(28) == 5);
    REQUIRE (bst->lookup(6) == 6);
    REQUIRE (bst->lookup(3) == 16);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 9);
    REQUIRE (bst->height() == 5);
    REQUIRE (bst->balance() == -1);

    REQUIRE (!bst->contains(9));
    REQUIRE (bst->contains(6));
    REQUIRE (bst->contains(3));

    bst->remove(3);
    REQUIRE (!bst->contains(3));
    REQUIRE (bst->balance() == -2);
    REQUIRE (bst->size() == 8);

    bst->remove(4);
    REQUIRE (!bst->contains(4));
    REQUIRE (bst->balance() == -3);
    REQUIRE (bst->size() == 7);

    bst->remove(7);
    REQUIRE (!bst->contains(7));
    REQUIRE (bst->balance() == -2);
    REQUIRE (bst->size() == 6);

    bst->insert(7, 50);
    REQUIRE (bst->contains(7));
    REQUIRE (bst->balance() == -2);
    REQUIRE (bst->size() == 7);

    bst->remove(8);
    REQUIRE (!bst->contains(8));
    REQUIRE (bst->balance() == -2);
    REQUIRE (bst->size() == 6);

    bst->clear();
    REQUIRE (bst->is_empty());

    bst->insert(8, 100);
    bst->insert(1, 88);
    bst->insert(30, 1);
    bst->insert(28, 5);
    bst->insert(6, 6);
    bst->insert(3, 16);

    REQUIRE (bst->contains(8));
    REQUIRE (bst->contains(1));
    REQUIRE (bst->contains(3));
    REQUIRE (!bst->contains(7));
    REQUIRE (bst->balance() == 1);
    REQUIRE (bst->size() == 6);

    delete bst;

}
//Non-Constant Iterator works but produced memory leak
/*
TEST_CASE( "BSTLEAF_ITER" ) {

    cop3530::BSTLEAF<int,int, compare, equality> * bst = new cop3530::BSTLEAF<int,int, compare, equality> ();
    bst->insert(5,3);
    bst->insert(10,3);
    bst->insert(2,3);
    bst->insert(8,3);

    int temp_contents [4] = {2, 5, 8, 10};

    int index = 0;

    for (cop3530::BSTLEAF<int, int, compare, equality>::iterator it = bst->begin(); it != bst->end(); ++it) {
        REQUIRE (temp_contents[index++] == (*it)->get_key());
    }

    delete bst;

}
*/
TEST_CASE( "BSTROOT" ) {

    cop3530::BSTROOT<char,int, compare, equality> * bst = new cop3530::BSTROOT<char,int, compare, equality> ();

    bst->insert('a',3);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 1);
    REQUIRE (bst->height() == 1);
    REQUIRE (!bst->balance());

    bst->insert('s', 65);
    bst->insert('e', 45);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 3);
    REQUIRE (bst->height() == 2);
    REQUIRE (!bst->balance());

    REQUIRE (bst->lookup('s') == 65);
    REQUIRE (bst->lookup('e') == 45);
    REQUIRE (!bst->contains('z'));

    bst->insert('r', 100);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 4);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == 1);
    REQUIRE (bst->contains('r'));

    bst->insert('c', 100);
    REQUIRE (bst->size() == 5);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == -1);

    bst->insert('h', 50);
    REQUIRE (bst->size() == 6);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == 0);

    bst->insert('i', 10);
    REQUIRE (bst->size() == 7);
    REQUIRE (bst->height() == 4);
    REQUIRE (bst->balance() == 1);

    bst->insert('n', 10);
    REQUIRE (bst->size() == 8);
    REQUIRE (bst->height() == 5);
    REQUIRE (bst->balance() == 2);

    REQUIRE (bst->contains('n'));
    REQUIRE (bst->contains('c'));
    REQUIRE (bst->contains('s'));
    REQUIRE (!bst->contains('z'));

    //Remove is same as Leaf BST
    bst->remove('c');
    REQUIRE (bst->size() == 7);
    REQUIRE (bst->height() == 5);
    REQUIRE (bst->balance() == 2);

    bst->remove('a');
    REQUIRE (bst->size() == 6);
    REQUIRE (bst->height() == 4);
    REQUIRE (bst->balance() == 1);

    bst->remove('n');
    REQUIRE (bst->size() == 5);
    REQUIRE (bst->height() == 4);
    REQUIRE (bst->balance() == 2);

    bst->insert('j', 1);
    REQUIRE (bst->height() == 4);
    bst->insert('k', 5);
    REQUIRE (bst->size() == 7); ///Not sure
  //  REQUIRE (bst->height() == 4);
    REQUIRE (bst->balance() == 2);

    delete bst;

}

TEST_CASE( "BSTRAND" ) {

    cop3530::BSTRAND<char,int, compare, equality> * bst = new cop3530::BSTRAND<char,int, compare, equality> ();

    bst->insert('a',3);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 1);
    REQUIRE (bst->height() == 1);
    REQUIRE (!bst->balance());

    bst->insert('b', 65);
    bst->insert('c', 45);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 3);

    REQUIRE (bst->lookup('b') == 65);
    REQUIRE (bst->lookup('c') == 45);
    REQUIRE (!bst->contains('z'));

    bst->insert('d', 100);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->size() == 4);
    REQUIRE (bst->contains('d'));

    bst->insert('e', 100);
    REQUIRE (bst->size() == 5);

    bst->insert('f', 50);
    REQUIRE (bst->size() == 6);

    bst->insert('g', 10);
    REQUIRE (bst->size() == 7);

    bst->insert('h', 10);
    REQUIRE (bst->size() == 8);

    REQUIRE (bst->contains('g'));
    REQUIRE (bst->contains('e'));
    REQUIRE (!bst->contains('s'));
    REQUIRE (!bst->contains('z'));

    //Remove is same as Leaf BST
    bst->remove('c');
    REQUIRE (bst->size() == 7);

    bst->remove('a');
    REQUIRE (bst->size() == 6);

    bst->remove('h');
    REQUIRE (bst->size() == 5);

    bst->insert('e', 1);
    bst->insert('g', 5);
    REQUIRE (bst->size() == 7);

    delete bst;

}

TEST_CASE( "AVL" ) {

    cop3530::AVL<int,int, compare, equality> * bst = new cop3530::AVL<int,int, compare, equality> ();

    REQUIRE (bst->is_empty());

    bst->insert(50,3);
    REQUIRE (bst->size() == 1);
    REQUIRE (bst->height() == 1);
    REQUIRE (bst->balance() == 0);

    REQUIRE (!bst->is_empty());

    bst->insert(55,10);
    REQUIRE (bst->size() == 2);
    REQUIRE (bst->height() == 2);
    REQUIRE (bst->balance() == -1);

    bst->insert(52,12);
    REQUIRE (bst->size() == 3);
    REQUIRE (bst->height() == 2);
    REQUIRE (bst->balance() == 0);

    REQUIRE (bst->contains(52));
    REQUIRE (bst->contains(55));
    REQUIRE (bst->contains(50));
    REQUIRE (!bst->contains(45));

    bst->insert(49,4);
    REQUIRE (bst->size() == 4);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == 1);

    bst->insert(48,56);
    REQUIRE (bst->size() == 5);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == 1);

    bst->insert(40,56);
    REQUIRE (bst->size() == 6);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == 0);

    bst->insert(100,100);
    REQUIRE (bst->size() == 7);
    REQUIRE (bst->height() == 4);
    REQUIRE (bst->balance() == -1);

    bst->insert(90,90);
    REQUIRE (bst->size() == 8);
    REQUIRE (bst->height() == 4);
    REQUIRE (bst->balance() == -1);

    REQUIRE (!bst->is_empty());
    REQUIRE (bst->contains(90));
    REQUIRE (bst->contains(100));
    REQUIRE (bst->contains(52));
    REQUIRE (!bst->contains(1000));
    REQUIRE (!bst->contains(2));

    REQUIRE (bst->lookup(48) == 56);
    REQUIRE (bst->lookup(49) == 4);
    REQUIRE (bst->lookup(55) == 10);

    //Testing Remove
    bst->remove(90);
    REQUIRE (!bst->contains(90));
    REQUIRE (bst->size() == 7);
    REQUIRE (bst->height() == 4);
    REQUIRE (bst->balance() == -1);

    bst->remove(50);
    REQUIRE (!bst->contains(50));
    REQUIRE (bst->size() == 6);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == 0);

    bst->remove(49);
    REQUIRE (!bst->contains(50));
    REQUIRE (bst->size() == 5);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == 0);

    bst->remove(48);
    REQUIRE (!bst->contains(48));
    REQUIRE (bst->size() == 4);
    REQUIRE (bst->height() == 3);
    REQUIRE (bst->balance() == -1);

    bst->remove(40);
    REQUIRE (!bst->contains(40));
    REQUIRE (bst->size() == 3);
    REQUIRE (bst->height() == 2);
    REQUIRE (bst->balance() == 0);

    delete bst;

}

///Implement string hash function as well
TEST_CASE( "HASHOPEN" ) {

    cop3530::HASHOPEN<int,int, hash_func, equality> * hash_map = new cop3530::HASHOPEN<int,int, hash_func, equality> ();
    hash_arr_size = hash_map->capacity() + 1;

    hash_map->insert(1, 3);

    REQUIRE (!hash_map->is_full());
    REQUIRE (!hash_map->is_empty());
    REQUIRE (hash_map->size() == 1);
    REQUIRE (hash_map->contains(1));
    REQUIRE (hash_map->lookup(1) == 3);

    hash_map->insert(5, 10);
    REQUIRE (!hash_map->is_full());
    REQUIRE (!hash_map->is_empty());
    REQUIRE (hash_map->size() == 2);
    REQUIRE (hash_map->contains(5));
    REQUIRE (hash_map->lookup(5) == 10);

    hash_map->clear();

    for (int i = 1; i <= 127; ++i)
        hash_map->insert(i, i + 10);

 //   hash_map->test();

    REQUIRE (hash_map->is_full());
    REQUIRE (!hash_map->is_empty());
    REQUIRE (hash_map->size() == 127);

    for (int i = 1; i <= 127; ++i) {
        REQUIRE (hash_map->contains(i));
        REQUIRE (hash_map->lookup(i) == i + 10);
    }

    REQUIRE (hash_map->load() == 1);

    for (int i = 1; i <= 127; ++i) {
        //std::cout << i << "  " << hash_map->lookup (i) << "\n";
        hash_map->remove(i);
        REQUIRE (!hash_map->contains (i));
        try {
            hash_map->lookup (i);
        }
        catch (std::runtime_error e) {}
        REQUIRE (hash_map->size() == 127 - i);
    }

    delete hash_map;

}

TEST_CASE( "HASHBUCKET" ) {

    cop3530::HASHBUCKET<int,int, hash_func, equality> * hash_map = new cop3530::HASHBUCKET<int,int, hash_func, equality> ();
    hash_arr_size = hash_map->capacity();

    hash_map->insert(1, 3);

    REQUIRE (!hash_map->is_full());
    REQUIRE (!hash_map->is_empty());
    REQUIRE (hash_map->size() == 1);
    REQUIRE (hash_map->contains(1));
    REQUIRE (hash_map->lookup(1) == 3);

    hash_map->insert(5, 10);
    REQUIRE (!hash_map->is_full());
    REQUIRE (!hash_map->is_empty());
    REQUIRE (hash_map->size() == 2);
    REQUIRE (hash_map->contains(5));
    REQUIRE (hash_map->lookup(5) == 10);

    hash_map->clear();

    for (int i = 1; i <= 127; ++i)
        hash_map->insert(i, i + 10);

    REQUIRE (!hash_map->is_empty());
    REQUIRE (hash_map->size() == 127);

    for (int i = 1; i <= 127; ++i) {
        REQUIRE (hash_map->contains(i));
        REQUIRE (hash_map->lookup(i) == i + 10);
    }

    REQUIRE (hash_map->load() == 1);

    for (int i = 1; i <= 127; ++i) {
        hash_map->remove(i);
        REQUIRE (!hash_map->contains (i));
        try {
            hash_map->lookup (i);
        }
        catch (std::runtime_error e) {}
        REQUIRE (hash_map->size() == 127 - i);
    }

     for (int i = 1; i <= 500; ++i) {
        hash_map->insert(i, i + 10);
        REQUIRE (hash_map->contains(i));
        REQUIRE (hash_map->lookup(i) == i + 10);
    }

    delete hash_map;

}

TEST_CASE( "HASHOPEN_Iter" ) {

   /* cop3530::HASHOPEN<int,int, &hash_func, &equality> * hash_map = new cop3530::HASHOPEN<int,int, &hash_func, &equality> ();
    hash_arr_size = hash_map->capacity() + 1;

    for (int i = 1; i <= 100; ++i)
        hash_map->insert(i, i + 10);

    int index = 0;
    //cop3530::HASHOPEN<int, int, &hash_func, &equality>::iterator it = hash_map->begin();
    for (cop3530::HASHOPEN<int, int, &hash_func, &equality>::iterator it = hash_map->begin(); it != hash_map->end(); ++it) {
        //REQUIRE (temp_contents[index++] == (*it)->get_key());
        std::cout << (*it)->get_key() << "  ";
    }

    index = 0;

    delete hash_map;
*/
}

