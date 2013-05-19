
#include <iostream>
#include <cassert>
#include <vector>

#include "skip_list_node.cpp"
#include "skip_list_random_generator.cpp"

namespace nomind {

using std::cout;
using std::endl;
using std::vector;

class MemAlloc {
private:
	static const size_t _block_size = 1048576;
	vector<char*> _block_list;

	size_t _mem_available;
	char* _mem_ptr;

public:
	MemAlloc();
	~MemAlloc();

	char* alloc(size_t bytes);
};

MemAlloc::MemAlloc() {
	_mem_available = 0;
	_mem_ptr = NULL;
}

char*
MemAlloc::alloc(size_t bytes) {
    //return new char[bytes];

	if(_mem_available >= bytes) {
		char *result = _mem_ptr;
		_mem_ptr += bytes;
		_mem_available -= bytes;
		return result;
	}
	_mem_ptr = new char[_block_size];
	_block_list.push_back(_mem_ptr);
	_mem_available = _block_size;
	return alloc(bytes);
}

MemAlloc::~MemAlloc() {
	int sz = _block_list.size();
	for(int i=0; i<sz; i++) {
		delete _block_list[i];
	}
}

template<class Value>
class SkipList {
private:
	int _max_level;
	SkipListNode<Value> *_head;
	SkipListRandomGenerator *_rand_gen;

	MemAlloc _mem_alloc;

	void _walk(Value val, SkipListNode<Value> **prev);

public:
	SkipList();
	~SkipList();

	void insert(Value val);
	bool find(Value val);
	void print();
};

template<class Value>
SkipList<Value>::SkipList() {
	char *rand_gen_alloc = _mem_alloc.alloc(sizeof(SimpleSkipListRandomGenerator));
	_rand_gen = new (rand_gen_alloc) SimpleSkipListRandomGenerator();
	_max_level = _rand_gen->getMaxLevel();

	char *head_alloc = _mem_alloc.alloc(sizeof(SkipListNode<Value>) * _max_level);
	_head = new (head_alloc) SkipListNode<Value>[_max_level];

	for(int i=0; i<_max_level; i++) {
		_head[i].setNext(_head + i);
	}
	for(int i=1; i<_max_level; i++) {
		_head[i].setChild(_head + i - 1);
	}
}

template<class Value>
SkipList<Value>::~SkipList() {

}

template<class Value>
void
SkipList<Value>::_walk(Value val, SkipListNode<Value> **prev) {
	SkipListNode<Value> *last = &_head[_max_level - 1];
	for(int i=_max_level - 1; i>=0; i--) {
		prev[i] = last;
		while(prev[i]->getNext() != (_head + i) && prev[i]->getNext()->getValue() <= val) {
			prev[i] = prev[i]->getNext();
		}
		last = prev[i]->getChild();
	}
	return;
}

template<class Value>
bool
SkipList<Value>::find(Value val) {
	bool ans = false;
	char *alloc = _mem_alloc.alloc(sizeof(SkipListNode<Value>*) * _max_level);
	SkipListNode<Value> **prev = new (alloc) SkipListNode<Value>*[_max_level];

	_walk(val, prev);
	if((*prev) != _head && (*prev)->getValue() == val) {
		ans = true;
	}

	return ans;
}

template<class Value>
void
SkipList<Value>::insert(Value val) {
	char *alloc = _mem_alloc.alloc(sizeof(SkipListNode<Value>*) * _max_level);
	SkipListNode<Value> **prev = new (alloc) SkipListNode<Value>*[_max_level];

	_walk(val, prev);
	if((*prev) != _head && (*prev)->getValue() == val) {
		return;
	}
	SkipListNode<Value> *parent = NULL;
	int level = _rand_gen->getRandomLevel();
	for(int i = level; i >= 0; i--) {
		char *alloc = _mem_alloc.alloc(sizeof(SkipListNode<Value>));
		SkipListNode<Value> *child = new (alloc) SkipListNode<Value>();
		child->setValue(val);

		child->setNext(prev[i]->getNext());
		prev[i]->setNext(child);

		if(parent != NULL) {
			parent->setChild(child);
		}
		parent = child;
	}

	return;
}

template<class Value>
void
SkipList<Value>::print() {
	SkipListNode<Value> *cur = _head->getNext();
	while(cur != _head) {
		cout<<cur->getValue()<<" ";
		cur = cur->getNext();
	}
	cout<<endl;
}

}

using nomind::SkipList;

using std::cout;
using std::endl;

void test(int setCount, int getCount) {
	int max_int = 1000000;
	int setter = 0, getter = 0;
	SkipList<int> skip_list;

	while(setter < setCount && getter < getCount) {
		int v = rand() % max_int;
		if(rand()%2 == 1) {
			skip_list.insert(v);
			setter++;
		} else {
			skip_list.find(v);
			getter++;
		}
	}

	while(setter < setCount) {
		int v = rand() % max_int;
		skip_list.insert(v);
		setter++;
	}

	while(getter < getCount) {
		int v = rand() % max_int;
		skip_list.find(v);
		getter++;
	}

	return;
}

