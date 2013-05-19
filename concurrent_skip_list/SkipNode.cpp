
#ifndef SKIP_NODE
#define SKIP_NODE

#include <cstdlib>
#include <atomic>

#include "Common.cpp"

class SkipNode {
public:
    KeyType key;
    ValType val;

    bool marker;

#ifdef CONCURRENT
    atomic<SkipNode*> next;
#else
    SkipNode* next;
#endif

    SkipNode *prev;

    int level;

    SkipNode() {
        next = NULL;
        prev = NULL;
    }

    SkipNode(KeyType& key, ValType& val, SkipNode *prev, SkipNode *next) {
        this->key = key;
        this->val = val;
        this->prev = prev;
        this->next = next;

        this->level = 0;
    }
};

#endif

