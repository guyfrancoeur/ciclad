// cmdline.h

/******************************
 * cmdline() [-i <in stream/file> | default stdin] [-o <output stream/file> | default stdout] [-w <window size> | default 1000] [-t <window time> | default 0]
 * -w sliding window size
 * -t time limit of window
 * -r ram size limit
 ******************************/

#ifndef CMDLINE_H
#define CMDLINE_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/time.h>
#include <sys/resource.h>

typedef unsigned int uint;
using namespace std;

struct Cmdline {
  Cmdline() : in(0), out(0), swin(1000), timewin(0), ram(0) {}
  uint in;
  uint out;
  uint swin;
  uint timewin;
  uint ram;
};

void readCmdline(Cmdline* _cmdline, char** argv, int argc);
void setRam(Cmdline* _cmdline, char** argv);

void readCmdline(Cmdline* _cmdline, char** argv, int argc) {
  fstream file;
  for (size_t index = 1; index < argc; ++index) {
    if (argv[index][0] == '-' && argv[index][2] == '\0') {
      switch (argv[index][1]) {
        case 'i':
          file.open(argv[index+1], ios::in);
          if(file.is_open()) {
            _cmdline->in=index+1;
            file.close();
          } else {
            exit(1);
          }
          break;
        case 'o':
          file.open(argv[index+1], ios::out);
          if(file.is_open()) {
            _cmdline->out=index+1;
            file.close();
          } else {
            exit(1);
          }
          break;
        case 'w':
          if(strlen(argv[index+1]) > 0 && strtol(argv[index+1],NULL,10) > 0) {
            _cmdline->swin=strtol(argv[index+1],NULL,10);
          } else {
            exit(1);
          }
          break;
        case 't':
          if(strlen(argv[index+1]) > 0 && strtol(argv[index+1],NULL,10) > 0) {
            _cmdline->timewin=strtol(argv[index+1],NULL,10);
          } else {
            exit(1);
          }
          break;
        case 'r':
          if(strlen(argv[index+1]) > 0 && strtol(argv[index+1],NULL,10) > 0) {
            _cmdline->ram=index+1;
          } else {
            exit(1);
          }
          break;
        default:
          cout << "Usage: " << argv[0] << " [-i <input stream/file> | default stdin] [-o <output stream/file> | default stdout] [-w <window size> | default 1000] [-t <window time> | default 0] [-r <ram limit> | default 50%]" << endl;
          exit(1);
      }
    }
  }
}

void setRam(Cmdline* _cmdline, char** argv) {
  uint ram;
  if(_cmdline->ram) {
    ram=strtol(argv[_cmdline->ram],NULL,10);
  }else{
    ram = 50;
  }
  struct rlimit limit;
  getrlimit(RLIMIT_AS, &limit);
  limit.rlim_cur=(limit.rlim_cur/100)*ram;
  limit.rlim_max=(limit.rlim_max/100)*ram;
  if(0 != setrlimit(RLIMIT_AS, &limit)){
    exit(1);
  }
}

#endif /* CMDLINE_H */
