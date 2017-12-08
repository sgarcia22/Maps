#ifndef KVPair_INCLUDED
#define KVPair_INCLUDED

namespace cop3530 {

template <typename key, typename value>
//Key-Value Pair Class
class KVPair {

public:

    KVPair() {}

    KVPair (key a, value b) : class_key (a), class_value (b) {
    }

    key get_key () const {
        return class_key;
    }

    key & get_value () {
        return class_value;
    }

    void set_key (key a) {
        class_key = a;
    }

    void set_value (value a) {
        class_value = a;
    }


private:
    key class_key;
    value class_value;
};
}

#endif
