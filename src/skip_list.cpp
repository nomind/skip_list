
#include <iostream>
#include <cassert>

#include "skip_list_node.cpp"
#include "skip_list_random_generator.cpp"

namespace nomind {

using std::cout;
using std::endl;

template<class Value>
class SkipList {
private:
	int _max_level;
	SkipListNode<Value> *_head;
	SkipListRandomGenerator *_rand_gen;

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
	_rand_gen = new SimpleSkipListRandomGenerator();
	_max_level = _rand_gen->getMaxLevel();
	_head = new SkipListNode<Value>[_max_level];

	for(int i=0; i<_max_level; i++) {
		_head[i].setNext(_head + i);
	}
}

template<class Value>
SkipList<Value>::~SkipList() {
	if(_head != NULL) {
		SkipListNode<Value> **next = new SkipListNode<Value>*[_max_level];
		for(int i=0; i<_max_level; i++) {
			next[i] = _head[i].getNext();
			_head[i].setNext(NULL);
		}
		delete[] next;
	}
	if(_rand_gen != NULL) {
		delete _rand_gen;
	}
}

template<class Value>
void
SkipList<Value>::_walk(Value val, SkipListNode<Value> **prev) {
	for(int i=0; i<_max_level; i++) {
		prev[i] = _head + i;
		while(prev[i]->getNext() != _head + i && prev[i]->getNext()->getValue() <= val) {
			prev[i] = prev[i]->getNext();
		}
	}
	return;
}

template<class Value>
bool
SkipList<Value>::find(Value val) {
	bool ans = false;
	SkipListNode<Value> **prev = new SkipListNode<Value>*[_max_level];
	_walk(val, prev);
	if((*prev) != _head && (*prev)->getValue() == val) {
		ans = true;
	}
	delete[] prev;
	return ans;
}

template<class Value>
void
SkipList<Value>::insert(Value val) {
	SkipListNode<Value> **prev = new SkipListNode<Value>*[_max_level];
	_walk(val, prev);
	if((*prev) != _head && (*prev)->getValue() == val) {
		return;
	}
	SkipListNode<Value> *parent = NULL;
	int level = _rand_gen->getRandomLevel();
	for(int i = level; i >= 0; i--) {
		SkipListNode<Value> *child = new SkipListNode<Value>();
		child->setValue(val);

		child->setNext(prev[i]->getNext());
		prev[i]->setNext(child);

		if(parent != NULL) {
			parent->setChild(child);
		}
		parent = child;
	}

	delete[] prev;
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

int main(int argc, char **argv) {
	/*SkipList<int> skip_list;
	skip_list.insert(2);
	skip_list.insert(3);
	skip_list.insert(3);
	skip_list.print();*/

	test(100000, 100000);

	return 0;
}
