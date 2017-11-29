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
    if(write(fd, "hello\n", 7) != 7) {
      printf(stdout, "Error: Writing hello %d new file failed\n", i);
      exit();
    }
  }
  struct stat st;
  fstat(fd, &st);
  if (st.size != 520) {
    printf(stdout, "File size is different from expected, size = %d\n", st.size);
  }
  printf(stdout, "Testing lseek(). Current file contents:\n");
  cat("mytestFile.txt");
  // the offset should be 70 before this: 10 writes of 7 characters
  if (lseek(fd, 80) == -1){
    printf(stdout, "lseek error\n");
  }
  if(write(fd, "hello\n", 7) != 7) {
      printf(stdout, "Error: Writing hello %d new file failed\n", i);
      exit();
  }
  // should see a gap of 10 zeroes before the last hello\n
  printf(stdout, "Gap of 10 inserted. Current file contents:\n");
  cat("mytestFile.txt");

  if(lseek(fd, -1) == 0){
    printf(stdout, "Error: lseek() allowed negative offset\n");
  }
  if(lseek(fd, 0x7FFFFFFF) == 0){
    printf(stdout, "Error: lseek() allowed offset outside the end of the file\n");
  }

  printf(stdout, "Test completed!\n");
  close(fd);
  exit();
}