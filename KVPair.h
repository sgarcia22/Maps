#ifndef KVPair_INCLUDED
#define KVPair_INCLUDED

namespace cop3530 {

template <typename key, typename value>
//Key-Value Pair Class
class KVPair {

public:

    KVPair() {}

    KVPair (key a, value b) : class_key (a) {
        class_value = b;
    }

    key get_key () {
        return class_key;
    }

    key get_value () {
        return class_value;
    }

private:
    key class_key;
    value class_value;
};
}

#endif
