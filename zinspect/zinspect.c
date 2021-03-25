/*
 * vim: expandtab ts=2 sw=2
 *
 * This is meant to inspect and print information from a daemontools
 * `supervise/status' file.
 *
 */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <err.h>

char status[18];

unsigned long pid;
unsigned char normallyup;
unsigned char want;
unsigned char paused;

int main(int argc, char **argv)
{
  struct stat st;
  int r;
  int fd;
  const char *x;

  normallyup = 0;
  if (stat("down", &st) == -1) {
    if (errno != ENOENT) {
      err(1, "unable to stat down");
    }
    normallyup = 1;
  }

  fd = open("supervise/ok", O_WRONLY|O_NONBLOCK);
  if (fd == -1) {
    if (errno == ENXIO) {
      printf("supervise not running, ");
    } else {
      err(1, "unable to open supervise/ok");
    }
  }
  close(fd);

  fd = open("supervise/status", O_RDONLY);
  if (fd == -1) {
    err(1, "unable to open supervise/status");
  }
  r = read(fd,status,sizeof status);
  close(fd);
  if (r < sizeof status) {
    const char *msg = "unable to read supervise/status";
    if (r == -1)
      err(1, msg);
    else
      errx(1, "%s, %s", msg, "bad format");
  }

  pid = (unsigned char) status[15];
  pid <<= 8; pid += (unsigned char) status[14];
  pid <<= 8; pid += (unsigned char) status[13];
  pid <<= 8; pid += (unsigned char) status[12];

  paused = status[16];
  want = status[17];

  if (pid)
    printf("up (pid %d)", pid);
  else
    printf("down");

  // printf("timestamp ignored");

  if (pid && !normallyup)
    printf(", normally down");
  if (!pid && normallyup)
    printf(", normally up");
  if (pid && paused)
    printf(", paused");
  if (!pid && (want == 'u'))
    printf(", want up");
  if (pid && (want == 'd'))
    printf(", want down");

  printf("\n");
}
