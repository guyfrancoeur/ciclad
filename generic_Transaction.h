#include <vector>
#include <iostream> //cout
#include <cstring>  //strtok
#include <string>   
#include <cstdio>   //stdin
#include <cstdlib>  //strtoul

#pragma warning(disable : 4996) //pour Visual C++ (std::strtok)

using namespace std;

template <class T> // T est le type des items (int, long, short); 
class Transaction { // itemset
public:
  string type();                                 //ex: (String) "add", "del"
  string crc();                                  //ex: (String) "1ade980c" le crc de la transaction
  size_t count();                                //nombre d'item dans la trx
  void load(char *, const char *, const short);  //pour charger une autre trx
  void clean();
  T next();
  Transaction(void);
  Transaction(char *, const char *, const short); //pointer vers le premier element, 1=avec crc/0=pas de crc a l'index [1]
  ~Transaction();
private:
  std::vector<T> __data;
  size_t __index;                                 //index de l'élément pointé pour le next();
  string __type;
  string __crc;
};

template <class T>
string Transaction<T>::type() {
  return __type;
};

template <class T>
string Transaction<T>::crc() {
  return __crc;
};

template <class T>
size_t Transaction<T>::count() {
  return __data.size();
};

template <class T>
T Transaction<T>::next() {
  return __data.at(__index++); //bound check
};

template <class T>
void Transaction<T>::clean(void) {
  __index = 0;
  __data.clear();
};

template <class T>
void Transaction<T>::load(char *_s, const char *_delims, const short _withcrc) {
  T v;
  clean();
  char *pch = std::strtok(_s, _delims);
  __type = std::string(pch); pch = strtok(0, _delims);
  if (_withcrc == 1) { __crc = std::string(pch); pch = strtok(0, _delims); }
  char *pEnd;
  while (pch != 0) {
    std::from_chars(pch, pch+strlen(pch), v);
    __data.insert(__data.end(), v);
    //old --> __data.insert(__data.end(),std::strtoul(pch,&pEnd,10));
    pch = strtok(0, _delims);
  }
};

template <class T>
Transaction<T>::Transaction() {
  clean();
}

template <class T>
Transaction<T>::Transaction(char *_s, const char *_delims, const short _crc) {
  clean();
  load(_s, _delims, _crc);
};

template <class T>
Transaction<T>::~Transaction() {
  __data.clear();
};
