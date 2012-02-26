#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int cnt;

void signalhandler(int sig_num) {
  printf("HANDLING: %d\n", cnt);
  exit(1);
}

void set_signalhandler(int SIG, void (*handler)(int)) {
  struct sigaction act;
  sigset_t smask;

  sigemptyset(&smask);
  sigaddset(&smask, SIG);

  act.sa_handler = handler;
  act.sa_mask = smask;
  act.sa_flags = 0;

  sigaction(SIG, &act, NULL);
}

int main() {
  cnt = 0;

  set_signalhandler(SIGINT, SIG_IGN);
  while (1) {
    if (cnt < 1000) {
      cnt++;
    } else {
      break;
    }
  }

  set_signalhandler(SIGINT, signalhandler);
  while (1) {
    if (cnt == 10000) {
      cnt = 1000;
    } else {
      cnt++;
    }
  }
}
