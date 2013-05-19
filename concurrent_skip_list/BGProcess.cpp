
#ifndef BGPROCESS
#define BGPROCESS

#include "SkipIndex.cpp"
#include "SkipNode.cpp"

class SkipList;

class BGProcess {
public:
    SkipIndex* top;
    int height;

    BGProcess(SkipIndex* top) {
        this->top = top;
        height = 1;
    }

    void start() {
        while(true) {
            raise_index();
        }
    }

    void raise_index() {
        #ifdef DEBUG
        cout<<"calling raise_index"<<endl;
        #endif

        int max = -1;
        SkipIndex *next = top;
        SkipIndex** first = new SkipIndex*[height];

        while(next != NULL) {

            #ifdef DEBUG
            cout<<"next is "<<next<<" with val "<<next->node->val<<endl;
            #endif

            max = max+1;
            first[max] = next;
            next = next->down;
        }

        bool inc_level = raise_nlevel(first[max]->node, first[max], 0);

        for(int i=max; i>0; i--) {
            inc_level = raise_ilevel(first[i], first[i-1], max-i+1);
        }

        if(inc_level) {
            height++;

            #ifdef DEBUG
            cout<<"increasing the height "<<height<<endl;
            #endif

            SkipIndex* new_index = new SkipIndex();
            new_index->down = top;
            new_index->node = top->node;
            top = new_index;
        }
    }

    bool raise_nlevel(SkipNode* prev, SkipIndex* prev_tall, int height) {
        int raised = false;
        SkipNode* index = prev->next;
        SkipNode* next;

        while(true) {
            
            if(index == NULL) {
                break;
            }

            next = index->next;
            if(next == NULL) {
                break;
            }
            
            while(index->val == (ValType)index) {
                prev->next = next;
                if(next == NULL) {
                    break;
                }
                index = next;
                next = next->next;
            }

            #ifdef DEBUG
            cout<<"index "<<index->val<<", prev "<<prev->val<<", next "<<next->val<<endl;
            #endif

            if(prev->level <= height &&
                index->level <= height &&
                next->level <= height) {
                
                #ifdef DEBUG
                cout<<"raising "<<index->val<<" to height "<<height+1<<endl;
                cout<<prev->level<<" "<<index->level<<" "<<next->level<<endl;
                #endif

                raised = true;
                SkipIndex* new_index = new SkipIndex();
                new_index->down = NULL;
                new_index->node = index;
                new_index->right = prev_tall->right;
                prev_tall->right = new_index;
                index->level = height + 1;
                prev_tall = new_index;

            }
    
            prev = index;
            index = index->next;
        }

        return raised;
    }

    bool raise_ilevel(SkipIndex* prev, SkipIndex* prev_tall, int height) {
        int raised = false;
        SkipIndex* index = prev->right;
        SkipIndex* next;

        while(true) {
            if(index == NULL) {
                break;
            }

            next = index->right;
            if(next == NULL) {
                break;
            }

            while(index->node->val == (ValType)index->node) {
                prev->right = next;
                if(next == NULL) {
                    break;
                }
                index = next;
                next = next->right;
            }

            if(prev->node->level <= height &&
                index->node->level <= height &&
                next->node->level <= height) {
                
                #ifdef DEBUG
                cout<<"raising "<<index->node->val<<" to height "<<height+1<<endl;
                cout<<prev->node->level<<" "<<index->node->level<<" "<<next->node->level<<endl;
                cout.flush();
                #endif

                raised = true;
                SkipIndex* new_index = new SkipIndex();
                new_index->down = index;
                new_index->node = index->node;
                new_index->right = prev_tall->right;
                prev_tall->right = new_index;
                index->node->level = height + 1;
                prev_tall = new_index;
            }
    
            prev = index;
            index = index->right;
        }

        return raised;
    }

};

void *start_bgp(void *top) {
    BGProcess bgp((SkipIndex*)top);
    bgp.start();
}

#endif

