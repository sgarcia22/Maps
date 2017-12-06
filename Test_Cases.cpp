//Include CATCH Framework
#define CATCH_CONFIG_MAIN

#include <iostream>
#include "catch.hpp"
//#include "ADTMAP.h"
#include "BSTLEAF.h"
#include "BSTROOT.h"
#include "BSTRAND.h"
#include "AVL.h"

template <typename element>
bool compare (element a, element b) {
    return (a > b ? true : false);
}

template <typename element>
bool equality (element a, element b) {
    return (a == b);
}

TEST_CASE( "BSTLEAF" ) {

    cop3530::BSTLEAF<int,int, &compare, &equality> * bst = new cop3530::BSTLEAF<int,int, &compare, &equality> ();

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

TEST_CASE( "BSTROOT" ) {

    cop3530::BSTROOT<char,int, &compare, &equality> * bst = new cop3530::BSTROOT<char,int, &compare, &equality> ();

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

    cop3530::BSTRAND<char,int, &compare, &equality> * bst = new cop3530::BSTRAND<char,int, &compare, &equality> ();

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

    cop3530::AVL<int,int, &compare, &equality> * bst = new cop3530::AVL<int,int, &compare, &equality> ();

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

TEST_CASE( "BSTLEAF_ITER" ) {



}
