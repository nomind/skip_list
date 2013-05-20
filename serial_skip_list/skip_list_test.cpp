
#include <iostream>
#include <cmath>
#include <sys/time.h>

using namespace std;

#include "skip_list.cpp"

void test_simple_inserts() {
    for(int i=10; i<17; i++) {
        struct timeval begin, end;
        gettimeofday(&begin, NULL);

        SkipList<int>* sl = new SkipList<int>();
        int cnt = (int)pow(2.0, i);
        
        for(int j=0; j<cnt; j++) {
            sl->insert(j);
        }

        gettimeofday(&end, NULL);
        int diff_sec = (int)difftime(end.tv_sec, begin.tv_sec) * 1000;
        int diff_milli = (int)difftime(end.tv_usec, begin.tv_usec)/1000;
        cout<<i<<" "<<diff_sec+diff_milli<<endl;
    }
    return;
}

int main() {
    test_simple_inserts();
    return 0;
}

