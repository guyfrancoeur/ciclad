//isci.cpp utils
//
#include <iostream>
#include <fstream>
#include "isci.h"

//write to file
//#ifdef _VECTOR
//int wf(vector<concept2> p, string s) {
//#else
//int wf(map<uint, concept2> p, string s) {
//#endif

template <class T>
void wf(T p, string s) {
	ofstream f;
	f.open(s);
	for (unsigned int i = 0; i < p.size(); ++i) {
        f << "id:" << p.at(i).id << " supp:" << p.at(i).supp << " {";
		for (unsigned int j = 0; j < p.at(i).itemset.size(); ++j) {
			f << p.at(i).itemset.at(j) << " ";
		}
		f << "}" << endl;
	}

	f.close();
	return;
}

// flat binary search tree using array (experimental)
void bst_insert(uint *array, uint _value) {
	int currentIndex = 0;
	//cout << "Adding: " << x;
	while (true) {
		if (array[currentIndex] == NULL) {
			array[currentIndex] = _value;
			//cout << " Inserted at index: " << currentIndex << endl;
			break;
		}
		else if (array[currentIndex] <= _value) {
			if (array[currentIndex] == _value) {
				//cout << "ERROR!-- Repeating element" << endl;
				break;
			}
			//else
			  //cout << " Right ";
			currentIndex = (2 * currentIndex + 2);
		}
		else if (array[currentIndex] >= _value) {
			if (array[currentIndex] == _value) {
				//cout << "ERROR!-- Repeating element" << endl;
				break;
			}
			//else
			  //cout << " Left ";
			currentIndex = (2 * currentIndex + 1);
		}
	}
}