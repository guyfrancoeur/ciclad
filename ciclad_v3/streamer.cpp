#include <cstdio>
#include <cstdlib>  //strtoul
#include <cstring>  //strcpy
#include <fcntl.h>  //open UNIX only
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h> //UNIX only
#include <vector>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
  int type=4; //1=fifo, 2=file, 4=stdout
  unsigned long w=1000;
  char myfifo[64] = "/tmp/myfifo";

  switch (argc) {
    case 1: { printf("Default window size is 1000\n"); break; }
    case 2: { w = strtoul(argv[1],NULL,10); break;}
    case 3: { w = strtoul(argv[1],NULL,10); strncpy(myfifo, argv[2], 63); break; }
    case 4: { w = strtoul(argv[1],NULL,10); strncpy(myfifo, argv[2], 63); if (strcmp(argv[3],"fifo")==0) type=1; if (strcmp(argv[3],"file")==0) type=2; remove(myfifo); break; }
    default : { fprintf(stderr, "Data is passed via stdin. Default output is stdout.\nUse & for NamedPipe.\nFor output directly into a file, use argv[2] and argv[3]=file\n");
                fprintf(stderr, "Usage example: %s 1000 /tmp/myfifo fifo|file|stdout < data.txt &\n",argv[0]);
                exit(1); break; }
  }
  fprintf(stderr, "Number of arguments is : %d",argc);
  int fd; FILE *f=stdout;
  vector<string> v;
  v.reserve(w);

  if (type==1) {
    mkfifo(myfifo, 0666); // Creating the named pipe(FIFO) mkfifo(<pathname>, <permission>)
  }
  char s[8192]={0};
  if (type==1) { fd = open(myfifo, O_WRONLY); }
  if (type==2) { f=fopen(myfifo,"w"); }
  unsigned int i=0;
  while (fgets(s, 8191, stdin) != NULL) {
    string str(s);
    v.push_back(str);
    char t[8192];
    sprintf(t,"add %s",s);
    if (type==1) { write(fd, t, strlen(t)); } else { fprintf(f,"%s",t); }
    ++i;
    if (i > w) {
      str = v.front();
      sprintf(t,"del %s",str.c_str());
      v.erase(v.begin());
      if (type==1) { write(fd, t, strlen(t)); } else { fprintf(f, "%s", t); }
    }
  }
  if (type==1) { write(fd, "end\n", 4); } else { fprintf(f, "end\n"); }
  if (type==1) close(fd);
  if (type==2) fclose(f);
  return 0;
}
