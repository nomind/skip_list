
#ifndef SKIP_LIST
#define SKIP_LIST

#include <cstdlib>
#include <atomic>
#include <iostream>

using namespace std;

#include <pthread.h>

#include "SkipNode.cpp"
#include "SkipIndex.cpp"

#include "Common.cpp"
#include "BGProcess.cpp"

class SkipList {
public:
    SkipIndex* top;

    enum Op {INSERT, DELETE, CONTAINS, GET};

    pthread_t bgp_thread;

    SkipList() {
        top = new SkipIndex();

        SkipNode* dummy = new SkipNode();
        dummy->val = -123456789L;
        dummy->key = -123456789L;

        top->node = dummy;

        init_bgp();
    }

    void init_bgp() {
        pthread_create(&bgp_thread, NULL, start_bgp, top);
    }

    ~SkipList() {
        pthread_join(bgp_thread, NULL);
    }

    int insert(KeyType key, ValType val) {
        return do_operation(INSERT, key, val);
    }

    ValType get(KeyType key) {
        return do_operation(GET, key, 1);
    }

    int do_operation(Op op, KeyType key, ValType val) {
        SkipNode* node;
        SkipIndex* item = top;
        int result;

        while(true) {
            SkipIndex* next_item = item->right;
            
            if(next_item == NULL || next_item->node->key > key) {
                next_item = item->down;
                if(next_item == NULL) {
                    node = item->node;
                    break;
                }
            } else if(next_item->node->key == key) {
                node = next_item->node;
                break;
            }

            item = next_item;
        }

        while(true) {
            ValType node_val;
            while(true) {
                node_val = node->val;
                if(node != (SkipNode*)node_val) {
                    break;
                }
                node = node->prev;
            }

            SkipNode* next = node->next;

            if(next != NULL && next->val == (ValType)next) {
                help_remove(node, next);
                continue;
            }

            if(next == NULL || next->key > key) {
                ValType next_val = 0;
                result = finish(op, key, val, node, node_val, next, next_val);
                if(result != FAILURE) {
                    break;
                }
                continue;
            }
            
            node = next;
        }

        return result;
    }

    void print() {
        SkipNode* cur = top->node->next;
        while(cur != NULL) {
            cout<<"("<<cur->key<<" "<<cur->val<<") ";
            cur = cur->next;
        }
        cout<<endl;
        return;
    }

    void help_remove(SkipNode* node, SkipNode* next) {

    }

    int finish(Op op, KeyType key, ValType val, SkipNode* node,
                    ValType node_val, SkipNode* next, ValType next_val) {
        if(op == INSERT) {
            return finish_insert(op, key, val, node, node_val, next);
        } else if(op == GET) {
            return finish_get(op, key, val, node, node_val, next);
        }
    }

    ValType finish_get(Op op, KeyType key, ValType val, SkipNode* node,
                    ValType node_val, SkipNode* next) {
        ValType result = DELETED;
        if(node->key == key) {
            if(node_val != DELETED) {
                result = node->val;
            }
        }
        return result;

    }

    int finish_insert(Op op, KeyType key, ValType val, SkipNode* node,
                    ValType node_val, SkipNode* next) {
        int result = DELETED;

        if(node->key == key) {
            if(node_val == DELETED) {
                if(compare_and_swap(node->val, node_val, val)) {
                    result = SUCCESS;
                }
            } else {
                result = FAILURE;
            }
        } else {

            SkipNode* new_node = new SkipNode(key, val, node, next);
            if(compare_and_swap(node->next, next, new_node)) {
                result = SUCCESS;
            }
        }

        return result;
    }

};

#endif

