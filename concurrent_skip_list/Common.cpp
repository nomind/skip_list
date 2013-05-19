
#ifndef COMMON
#define COMMON

#define CHECK_ON
#define CONCURRENT
//#define DEBUG

#define SUCCESS 1
#define FAILURE -1
#define DELETED -2

#include <atomic>

using namespace std;

typedef long long KeyType;
typedef long long ValType;

template<class T>
bool compare_and_swap(T& val, T& exp_val, T& new_val) {

    bool result = false;

    if(val == exp_val) {
        val = new_val;
        result = true;
    }

    return result;
}

template<class T>
bool compare_and_swap(atomic<T>& val, T& exp_val, T& new_val) {

    bool result = false;

    result = val.compare_exchange_weak(exp_val, new_val);

    return result;
}

#endif

