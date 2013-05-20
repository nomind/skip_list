
#include <iostream>
#include <cmath>
#include <cassert>
#include <sys/time.h>

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

    int cnt = end - start;
    for(int i=start; i<end; i++) {
        int x = rand()%cnt + start;
        sl->get(x);
    }
}

void scalability_insert_test() {
    int insert_cnt = pow(2, 16);
    for(int i=4; i<24; i+=4) {
        struct timeval begin, end;
        gettimeofday(&begin, NULL);

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

        gettimeofday(&end, NULL);
        int diff_sec = difftime(end.tv_sec, begin.tv_sec) * 1000;
        int diff_milli = difftime(end.tv_usec, begin.tv_usec)/1000;
        cout<<i<<" "<<diff_sec+diff_milli<<endl;
    }
    return;
}

void scalability_get_test() {
    int insert_cnt = pow(2, 16);
    SkipList* sl = new SkipList();
    for(int j=0; j<insert_cnt; j++) {
        sl->insert(j, j);
    }

    for(int i=4; i<24; i+=4) {
        struct timeval begin, end;
        gettimeofday(&begin, NULL);

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

        gettimeofday(&end, NULL);
        int diff_sec = difftime(end.tv_sec, begin.tv_sec) * 1000;
        int diff_milli = difftime(end.tv_usec, begin.tv_usec)/1000;
        cout<<i<<" "<<diff_sec+diff_milli<<endl;
    }
    return;
}

void test_simple_inserts() {
    for(int i=10; i<13; i++) {
        struct timeval begin, end;
        gettimeofday(&begin, NULL);

        SkipList* sl = new SkipList();
        int cnt = pow(2, i);

        for(int j=0; j<cnt; j++) {
            sl->insert(j, j);
        }

        gettimeofday(&end, NULL);
        int diff_sec = difftime(end.tv_sec, begin.tv_sec) * 1000;
        int diff_milli = difftime(end.tv_usec, begin.tv_usec)/1000;
        cout<<i<<" "<<diff_sec+diff_milli<<endl;
    }
    return;
}

void test_simple_get() {
    for(int i=10; i<17; i++) {
        SkipList* sl = new SkipList();
        int cnt = pow(2, i);
        
        for(int j=0; j<cnt; j++) {
            sl->insert(j, j);
        }

        struct timeval begin, end;
        gettimeofday(&begin, NULL);

        for(int j=0; j<cnt; j++) {
            int x = rand()%cnt;
            int v = sl->get(x);
            //assert(v == j);
        }

        gettimeofday(&end, NULL);
        int diff_sec = difftime(end.tv_sec, begin.tv_sec) * 1000;
        int diff_milli = difftime(end.tv_usec, begin.tv_usec)/1000;
        cout<<i<<" "<<diff_sec+diff_milli<<endl;
    }
    return;
}

int main() {
    test_simple_inserts();
    //test_simple_get();
    //scalability_insert_test();
    //scalability_get_test();
    return 0;
}

