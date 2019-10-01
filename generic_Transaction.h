#include <vector>
#include <iostream> //cout
#include <cstring>  //strtok
#include <cstdio>   //stdin

using namespace std;

template <class T>
class Transaction { // itemset
  // T est le type des items;
  // U est le type pour determiner le type de trx;
public:
  String type;          //ex: (String) "add", "del"
  String crc;           //ex: (String) "1ade980c" le crc de la transaction
  size_t count();       //nombre d'item dans la trx
  void load();          //pour charger une autre trx
  void clean();
  T next();
  Transaction(char *, char *, short); //pointer vers le premier element, 1=avec crc/0=pas de crc a l'index [1]
  ~Transaction();
private:
  std::vector<T> data;
  size_t index;             //index de l'element pointé pour le next();
};

template <class T>
size_t Transaction<T>::count() {
  return data.size;
};

template <class T>
T Transaction<T>::next() {
  return data.at(index++);
};

template <class T>
void Transaction<T>::clean() {
  index=0;
  data.clear();
};

template <class T>
void Transaction<T>::load(const char *_s, const char *_delims, const short _crc) {
  if (index != 0) clean();
  char *pch = strtok(_s, _delims);
  type = std::string(pch++);
  if (_crc == 1) crc = std::string(pch++);
  while (pch != 0) {
    data.push_back(strtoul(_s));
    pch = strtok(0, _delims);
  }  
};

template <class T>
Transaction<T>::Transaction() {
  clean();
}

template <class T>
Transaction<T>::Transaction(const char *_s, const char *_delims, const short _crc) {
  clean();
  load(_s, _delims, _crc);
};

template <class T>
Transaction<T>::~Transaction() {
  data.clear();
};
