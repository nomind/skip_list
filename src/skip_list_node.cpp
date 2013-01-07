
#include <cstring>

namespace nomind {

template<class Value>
class SkipListNode {
private:
	Value _value;
	SkipListNode<Value> *_next;
	SkipListNode<Value> *_child;

public:
	SkipListNode();
	~SkipListNode();

	Value getValue();
	void setValue(Value value);
	SkipListNode<Value>* getNext();
	void setNext(SkipListNode<Value> *next);
	SkipListNode<Value>* getChild();
	void setChild(SkipListNode<Value> *child);
};

template<class Value>
SkipListNode<Value>::SkipListNode() {
	_next = NULL;
	_child = NULL;
}

template<class Value>
SkipListNode<Value>::~SkipListNode() {
	if(_next != NULL) {
		//delete _next;
	}
}

template<class Value>
SkipListNode<Value>*
SkipListNode<Value>::getNext() {
	return _next;
}

template<class Value>
SkipListNode<Value>*
SkipListNode<Value>::getChild() {
	return _child;
}

template<class Value>
Value
SkipListNode<Value>::getValue() {
	return _value;
}

template<class Value>
void
SkipListNode<Value>::setNext(SkipListNode<Value> *next) {
	_next = next;
	return;
}

template<class Value>
void
SkipListNode<Value>::setChild(SkipListNode<Value> *child) {
	_child = child;
	return;
}

template<class Value>
void
SkipListNode<Value>::setValue(Value value) {
	_value = value;
	return;
}

}
