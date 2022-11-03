#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

  if (argc != 2) {
    write(1, "inputerr", 8);
    exit(1);
  }
  int time = atoi(argv[1]);
  sleep(time);
  exit(0);
}
