#include <iostream>
#include "BSTLEAF.h"

using namespace std;
/*
    make a non const
    make const
    copy non const to const

*/

int main()
{
    cop3530::BSTLEAF<int,int> * bst = new cop3530::BSTLEAF<int,int> ();
    bst->insert(5, 3);
    bst->insert(7, 65);
    bst->insert(4, 45);

	cop3530::BSTLEAF<int,int> const * bst_const = bst;

   for (cop3530::BSTLEAF<int,int>::const_iterator i = bst_const->begin(); i != bst_const->end(); ++i) {
    std::cout << (*i)->get_key() << "  ";
    }


   /* cout << "size: " << bst->size() << endl;
    //bst->remove(5);
    cout << "lookup (4): " << bst->lookup(4) << endl;
    cout << "is empty: " << bst->is_empty() << endl;
  //  bst->clear();
    cout << "is empty: " << bst->is_empty() << endl;

*/
  //Constructors

    cop3530::BSTLEAF<int,int> * bst2 = bst;
    cout << "lookup (5): " << bst2->lookup(5) << endl;














  //  delete bst;
   /* cop3530::BSTROOT<int,int> * bst = new cop3530::BSTROOT<int,int> ();
    bst->insert(5, 3);
    bst->insert(7, 65);
    bst->insert(4, 45);
    cout << bst->lookup(5) << endl;
    cout << bst->lookup(4) << endl;*/

 /*   cop3530::BSTRAND<int,int> * bst = new cop3530::BSTRAND<int,int> ();
    bst->insert(5, 3);
    bst->insert(7, 65);
    bst->insert(4, 45);
    bst->insert(3, 23);
    bst->insert(2, 15);
    bst->insert(1, 445);
    cout << bst->lookup(5) << endl;
    cout << bst->lookup(7) << endl;
    cout << bst->lookup(4) << endl;
*/
/*
 cop3530::AVL<int,int> * bst = new cop3530::AVL<int,int> ();
  bst->insert(5, 3);
    bst->insert(7, 65);
      bst->insert(4, 45);
        bst->insert(3, 23);
        bst->insert(2, 15);
    cout << bst->lookup(2) << endl; */
    return 0;
}
