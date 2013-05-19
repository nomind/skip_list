
#ifndef SKIP_INDEX
#define SKIP_INDEX

#include <cstdlib>

#include "SkipNode.cpp"

class SkipIndex {
public:
    SkipIndex* right;
    SkipIndex* down;

    SkipNode *node;

    SkipIndex() {
        right = NULL;
        down = NULL;
        node = NULL;
    }
};

#endif

