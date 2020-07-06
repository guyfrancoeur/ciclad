#include <cstdio>
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
  int fd;
  vector<string> v;
  v.reserve(100);
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
    if (i > 1000) {
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
