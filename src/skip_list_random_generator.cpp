
#include <cstdlib>
#include <iostream>

namespace nomind {

using std::cout;
using std::endl;

class SkipListRandomGenerator {
private:
	const static int _DEFAULT_MAX_LEVEL = 12;

protected:
	int max_level;
	virtual void initMaxLevel();

public:
	SkipListRandomGenerator();
	virtual ~SkipListRandomGenerator();

	virtual void stats() = 0;
	virtual int getMaxLevel() = 0;
	virtual int getRandomLevel() = 0;
};

SkipListRandomGenerator::SkipListRandomGenerator() {
	/**
	 * The implementation classes, if they want to customize max_level
	 * should override the virtual method initMaxLevel
	 */

	initMaxLevel();
}

void
SkipListRandomGenerator::initMaxLevel() {
	max_level = _DEFAULT_MAX_LEVEL;
}

SkipListRandomGenerator::~SkipListRandomGenerator() {

}

class SimpleSkipListRandomGenerator : public SkipListRandomGenerator {
private:
	int *_level_counter;
public:
	SimpleSkipListRandomGenerator();

	void stats();
	int getMaxLevel();
	int getRandomLevel();

};

SimpleSkipListRandomGenerator::SimpleSkipListRandomGenerator() {
	_level_counter = new int[max_level];
	for(int i=0; i<max_level; i++) {
		_level_counter[i] = 0;
	}
}

int
SimpleSkipListRandomGenerator::getMaxLevel() {
	return max_level;
}

int
SimpleSkipListRandomGenerator::getRandomLevel() {
	int level = 0;
	while(rand()%2 == 1 && level < max_level - 1) {
		level++;
	}
	_level_counter[level]++;
	return level;
}

void
SimpleSkipListRandomGenerator::stats() {
	for(int i=0; i<max_level; i++) {
		cout<<i<<" "<<_level_counter[i]<<endl;
	}
}

}
