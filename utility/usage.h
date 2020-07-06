#ifndef _UTILITY_USAGE_H
#define _UTILITY_USAGE_H

#ifdef LINUX

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>  //printf
#include <string.h> //memset
#include <stdlib.h> //malloc

//a la C
void gu_all(struct rusage *_u) {
  getrusage(RUSAGE_SELF, _u);
};

void pu_ram(struct rusage &_u) {
  printf("Memory usage : %ld K\n", _u.ru_maxrss);
};

void pu_utime(struct rusage &_u) {
  struct timeval end = _u.ru_utime;
  printf("User time (sec) : %ld.%lds\n", end.tv_sec, end.tv_usec);
};

void pu_stime(struct rusage &_u) {
  struct timeval end = _u.ru_stime;
  printf("System time (sec) : %ld.%lds\n", end.tv_sec, end.tv_usec);
};
#endif

#ifdef _WIN32
  #include <windows.h>
  #include <psapi.h>

void pu_ram() {
  PROCESS_MEMORY_COUNTERS_EX info;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&info, sizeof(info));
  //<< "WorkingSet " << info.WorkingSetSize / 1024 << " K,";
  //<< "PeakWorkingSet " << info.PeakWorkingSetSize / 1024 << " K';
  //PrivateSet
  std::cout << "Memory usage : " << info.PrivateUsage / 1024 << " K" << endl;
}
#endif

#endif

/*
int main() {

  int i = 0;
  struct rusage r_usage;
  while (++i <= 10) {
    void* m = malloc(20 * 1024 * 1024);
    memset(m, 0, 20 * 1024 * 1024);
    getrusage(RUSAGE_SELF, &r_usage);
    printf("Memory usage = %ld\n", r_usage.ru_maxrss);
    sleep(3);
    free(m);
  }
  printf("\nAllocated memory, sleeping ten seconds after which we will check again...\n\n");
  sleep(10);
  getrusage(RUSAGE_SELF, &r_usage);
  printf("Memory usage = %ld\n", r_usage.ru_maxrss);


  struct rusage usage;
  struct timeval start, end;
  long k = 0;

  getrusage(RUSAGE_SELF, &usage);
  start = usage.ru_utime;
  for (long i = 0; i < 1000000; i++) {
    // Double loop for more interesting results.
    for (long j = 0; j < 10000; j++) {
      k += 20;
    }
  }
  getrusage(RUSAGE_SELF, &usage);
  end = usage.ru_utime;

  printf("Started at: %ld.%lds\n", start.tv_sec, start.tv_usec);
  printf("user time end: %ld.%lds\n", end.tv_sec, end.tv_usec);
  getrusage(RUSAGE_SELF, &usage);
  end = usage.ru_stime;
  printf("system time end: %ld.%lds\n", end.tv_sec, end.tv_usec);
  return 0;
} */
