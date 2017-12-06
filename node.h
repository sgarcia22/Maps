#ifndef NODE_INCLUDED
#define NODE_INCLUDED

template <typename key, typename value>
struct node {
    key priority;
    value data;
    node * left = nullptr;
    node * right = nullptr;
};


#endif
