#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

int stdout = 1;

int
main (int argc, char* argv[]) {
//   int fd = open("output.txt", O_CREATE | O_RDWR);
  int fd = open("mytestFile.txt", O_CREATE | O_RDWR | O_EXTENT);
  if (fd <= 1) {
    printf(stdout, "Issue creating file\n");
  }
  
  printf(stdout, "Starting test\n");
  int i;
  for (i = 0; i < 10; i++) {
    if(write(fd, "hello\n", 8) != 8) {
      printf(stdout, "Error: Writing hello %d new file failed\n", i);
      exit();
    }
  }
  struct stat st;
  fstat(fd, &st);
  if (st.size != 520) {
    printf(stdout, "File size is different from expected, size = %d\n", st.size);
  }
  printf(stdout, "Test completed!\n");
  close(fd);
  exit();
}