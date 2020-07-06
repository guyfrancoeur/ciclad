#include <cstdio>
#include <cstdlib>  //strtoul
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
  unsigned long w=1000;
  if (argc == 1) printf("Default window size is 1000\n");
  else  if (argc > 1) { w = strtoul(argv[1],NULL,10); }
  else { printf("Data is passed via stdin, using & help!\nUsage example: %s 1000 < data.txt &\n",argv[0]); exit(1); }
  int fd;
  vector<string> v;
  v.reserve(w);
  char myfifo[64] = "/tmp/myfifo";

  mkfifo(myfifo, 0666); // Creating the named pipe(FIFO) mkfifo(<pathname>, <permission>)

  char s[8192]={0};
  fd = open(myfifo, O_WRONLY);
  unsigned int i=0;
  while (fgets(s, 8191, stdin) != NULL) {
    string str(s);
    v.push_back(str);
    char t[8192];
    sprintf(t,"add %s",s);
    write(fd, t, strlen(t));
    ++i;
    if (i > w) {
      str = v.front();
      sprintf(t,"del %s",str.c_str());
      v.erase(v.begin());
      write(fd, t, strlen(t));
    }
  }
  write(fd, "end\n", 4);
  close(fd);
  return 0;
}
