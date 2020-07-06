#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector> 
#include <queue>
#include <time.h>

#include "../newciclad/transaction.h"
#include "../newciclad/concept.h"
#include "../newciclad/trienode.h"
#include "../newciclad/CicladAdd.hpp"
#include "../newciclad/CicladRmv.hpp"

#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

uint32_t start_ciclad(char* const _fileSource, const uint32_t _windowSize, const uint32_t _maxItem);
int readfile(char *fn, std::queue<TRANSACTION> &T);