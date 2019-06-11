#ifndef _ISCI_H_
#define _ISCI_H_

#include <map>
#include <vector>

typedef unsigned int uint;
typedef unsigned short ushort;

using namespace std;

struct concept2 {
	concept2() : id(0), supp(0), size(0) {}
	concept2(uint _id, uint _supp, ushort _size) : id(_id), supp(_supp), size(_size) {}
	concept2(uint _id, uint _supp, ushort _size, std::vector<uint> _itemset) : id(_id), supp(_supp), size(_size), itemset(_itemset) {}
	uint id;
	uint supp;
	ushort size;
	std::vector<uint> itemset;
};

//#ifdef _VECTOR
//  typedef vector<concept2> fCI2_t;
//  int wf(vector<concept2> p, string s); //function write to file
//#else
//  typedef map<uint, concept2> fCI2_t;
//  int wf(map<uint, concept2> p, string s); //function write to file
//#endif

template <class T> void wf(T p, string s);
void bst_insert(uint *array, uint _value); //function binary search tree insert

#endif // _ISCI_H_