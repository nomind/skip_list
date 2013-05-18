
#include <iostream>
#include <cmath>

using namespace std;

#include "skip_list.cpp"

void test_simple_inserts() {
    for(int i=15; i<17; i++) {
        SkipList<int>* sl = new SkipList<int>();
        int cnt = pow(2, i);
        time_t start, end;
        time(&start);
        for(int j=0; j<cnt; j++) {
            sl->insert(j);
        }
        time(&end);
        cout<<i<<" "<<difftime(end, start)<<endl;
    }
    return;
}

int main() {
    test_simple_inserts();
    return 0;
}

