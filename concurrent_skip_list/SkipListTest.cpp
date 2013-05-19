
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

#include "Common.cpp"
#include "SkipList.cpp"

struct WorkerData {
    SkipList* sl;
    int start;
    int insert_cnt;
};

void *worker(void* worker_data) {
    WorkerData* data = (WorkerData*)worker_data;
    int start = data->start;
    int end = start + data->insert_cnt;
    SkipList* sl = data->sl;

    for(int i=start; i<end; i++) {
        sl->insert(i, i);                       
    }
}

void *worker_get(void* worker_data) {
    WorkerData* data = (WorkerData*)worker_data;
    int start = data->start;
    int end = start + data->insert_cnt;
    SkipList* sl = data->sl;

    for(int i=start; i<end; i++) {
        sl->get(i);                       
    }
}

void scalability_insert_test() {
    int insert_cnt = pow(2, 15);
    for(int i=2; i<9; i+=2) {
        time_t start_t, end_t;
        time(&start_t);

        int t_cnt = i; //pow(2, i);
        pthread_t thread[t_cnt];

        SkipList* sl = new SkipList();
        int insert_per_thread = insert_cnt/t_cnt;
        int start = 0;
    
        for(int j=0; j<t_cnt; j++) {
            WorkerData *data = new WorkerData();
            data->sl = sl;
            data->start = start;
            data->insert_cnt = insert_per_thread;
            start += insert_per_thread;

            pthread_create(thread+j, NULL, worker, data);
        }

        for(int j=0; j<t_cnt; j++) {
            pthread_join(thread[j], NULL);
        }

        //sl->print();

        time(&end_t);
        cout<<i<<" "<<difftime(end_t, start_t)<<endl;
    }
    return;
}

void scalability_get_test() {
    int insert_cnt = pow(2, 15);
    SkipList* sl = new SkipList();
    for(int j=0; j<insert_cnt; j++) {
        sl->insert(j, j);
    }

    for(int i=2; i<9; i+=2) {
        time_t start_t, end_t;
        time(&start_t);

        int t_cnt = i; //pow(2, i);
        pthread_t thread[t_cnt];

        int insert_per_thread = insert_cnt/t_cnt;
        int start = 0;
    
        for(int j=0; j<t_cnt; j++) {
            WorkerData *data = new WorkerData();
            data->sl = sl;
            data->start = start;
            data->insert_cnt = insert_per_thread;
            start += insert_per_thread;

            pthread_create(thread+j, NULL, worker_get, data);
        }

        for(int j=0; j<t_cnt; j++) {
            pthread_join(thread[j], NULL);
        }

        //sl->print();

        time(&end_t);
        cout<<i<<" "<<difftime(end_t, start_t)<<endl;
    }
    return;
}

void test_simple_inserts() {
    for(int i=15; i<17; i++) {
        SkipList* sl = new SkipList();
        int cnt = pow(2, i);
        time_t start, end;
        time(&start);
        for(int j=0; j<cnt; j++) {
            sl->insert(j, j);
        }

        //sl->print();

        time(&end);
        cout<<i<<" "<<difftime(end, start)<<endl;
    }
    return;
}

void test_simple_get() {
    for(int i=15; i<17; i++) {
        SkipList* sl = new SkipList();
        int cnt = pow(2, i);
        
        for(int j=0; j<cnt; j++) {
            sl->insert(j, j);
        }

        //sl->print();

        time_t start, end;
        time(&start);

        for(int j=0; j<cnt; j++) {
            int v = sl->get(j);
            //assert(v == j);
        }

        time(&end);
        cout<<i<<" "<<difftime(end, start)<<endl;
    }
    return;
}

int main() {
    //test_simple_inserts();
    //test_simple_get();
    //scalability_insert_test();
    scalability_get_test();
    return 0;
}

