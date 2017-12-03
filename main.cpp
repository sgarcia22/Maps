#include <iostream>
#include "ADTMAP.h"

using namespace std;

int main()
{
  /*  cop3530::BSTLEAF<int,int> * bst = new cop3530::BSTLEAF<int,int> ();
    bst->insert(5, 3);
    bst->insert(7, 65);
    bst->insert(4, 45);

    //bst->print();
    //cout << bst->lookup(5) << endl;
    bst->remove(5);
    cout << bst->lookup(4) << endl;
    */

   /* cop3530::BSTROOT<int,int> * bst = new cop3530::BSTROOT<int,int> ();
    bst->insert(5, 3);
    bst->insert(7, 65);
    bst->insert(4, 45);
    cout << bst->lookup(5) << endl;
    cout << bst->lookup(4) << endl;*/

    cop3530::BSTRAND<int,int> * bst = new cop3530::BSTRAND<int,int> ();
    bst->insert(5, 3);
    bst->insert(7, 65);
    bst->insert(4, 45);
    bst->insert(3, 23);
    bst->insert(2, 15);
    bst->insert(1, 445);
    cout << bst->lookup(5) << endl;
    cout << bst->lookup(7) << endl;
    cout << bst->lookup(4) << endl;

    return 0;
}
