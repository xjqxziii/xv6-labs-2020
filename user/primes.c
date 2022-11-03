#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PREAD 0
#define PWRITE 1

void
node(int lread){

    int number;
    int readresult = read(lread, &number, 1);
    if (readresult == 0) {
        close(lread);
        return;
    }
    printf("prime %d\n", number); // 第一个先输出

    // 创建管段
    int fd_r[2];
    pipe(fd_r);

    int pid = fork();
    if (pid == 0) // child
    {
        close(lread);
        close(fd_r[PWRITE]);

        node(fd_r[PREAD]);

        close(fd_r[PREAD]);
        exit(0);
    } else { // parent
        close(fd_r[PREAD]);

        int tmp;
        int readresult = read(lread, &tmp, 1);

        while ((readresult != 0)) { // 读到了数
            if (tmp % number) // 没除尽
            {
                write(fd_r[PWRITE], &tmp, 1);
            }
            readresult = read(lread, &tmp, 1);
        }

        close(lread);
        close(fd_r[PWRITE]);
        wait(0);
    }
}

int
main(int argc, char *argv[])
{
    int fd_l[2];
    pipe(fd_l);
    int pid = fork();
    if (pid > 0) // father
    {
        close(fd_l[PREAD]);
        for (int i = 2; i <= 35; ++i) // 塞数据
        {
            write(fd_l[PWRITE], &i, 1);
        }
        close(fd_l[PWRITE]);
        wait(0);
        exit(0);
    } else { // child
        close(fd_l[PWRITE]);

        node(fd_l[PREAD]);

        close(fd_l[PREAD]);
        exit(0);
    }
}
