#include <iostream>
#include <vector>
#include <algorithm>
#include "huffman_tree.hpp"

typedef Crypt::Messages::iterator It;

int main () {
    int N;
    std::cin >> N;
    std::cin.ignore();
    Crypt::Messages msgs;
    while ( N-- > 0 ) {
        double prob;
        std::string call;
        std::cin >> prob >> call;
        Crypt::Message* msg = new Crypt::Message(call, prob);
        msgs.push_back(msg);
    }
    // view code table for huffman encode
    Crypt::view_table(msgs);
    // view code table for blocked huffman encode
    Crypt::view_2blocked_table(msgs);
    Crypt::view_3blocked_table(msgs);
    for (It i = msgs.begin(); i != msgs.end(); i++) {
        delete *i;
    }
}
