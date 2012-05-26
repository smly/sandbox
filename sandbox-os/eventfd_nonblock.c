#include <sys/eventfd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>

int main(int argc, char** argv) {
  // int efd = eventfd(0, 0);
  int efd = eventfd(0, EFD_NONBLOCK);
  uint64_t val = 0;
  ssize_t st;

  switch( fork() ) {
    case 0: // child
      sleep(2);

      val++;
      write(efd, &val, sizeof(uint64_t));
      printf("child write(): eventfd: %llu\n", val);

      close(efd);
      exit(EXIT_SUCCESS);
    default:
      while (1) {
        printf("parent before read(): eventfd: %llu\n", val);

        st = read(efd, &val, sizeof(uint64_t));
        printf("parent after read(): eventfd: %llu\n", val);

        if (st == -1 && errno == EAGAIN) {
          sleep(1);
          continue;
        }

        close(efd);
        exit(EXIT_SUCCESS);
      }
  }

  exit(EXIT_SUCCESS);
}
