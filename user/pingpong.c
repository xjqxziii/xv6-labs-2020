#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PREAD 0
#define PWRITE 1
int
main(int argc, char *argv[])
{
  int p_c2p[2];
  int p_p2c[2];
  pipe(p_c2p);
  pipe(p_p2c);
  int pid = fork();
  if (pid == 0) { // child
    char res[1];
    read(p_p2c[PREAD], res, 1);
    printf("%d: received ping\n", getpid());
    write(p_c2p[PWRITE], "1", 1);
  } else if (pid > 0) { // father
    write(p_p2c[PWRITE], "1", 1);
    char res[1];
    read(p_c2p[PREAD], res, 1);
    printf("%d: received pong\n", getpid());

  } else {
    printf("forkerr\n");
  }
  exit(0);
}
