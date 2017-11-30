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
  int fd = open("mytestFile.txt", O_CREATE | O_RDWR | O_EXTENT);
  int fd2 = open("mytestFile2.txt", O_CREATE | O_RDWR | O_EXTENT);
  struct stat st, st2; //for fstat
  if (fd <= 1) {
    printf(stdout, "Issue creating file1\n");
  }
  
  if (fd2 <= 1) {
    printf(stdout, "Issue creating file2\n");
  }
  printf(stdout, "**************** Starting test for file1 ********************\n");
  int i;
  for (i = 0; i < 65; i++) {
    if(write(fd, "abcdef\n", 8) != 8) {
      printf(stdout, "Error: Writing abcdef to new file1 line  %d failed\n", i);
      exit();
    }
    else{printf(stdout, "Writing abcdef to line%d of file1\n", i);}
  }

  fstat(fd, &st);
  printf(stdout, "File size i= %d\n", st.size);
  printf(stdout, "File type = %d\n", st.type);
  printf(stdout, "File system device = %d\n", st.dev);
  printf(stdout, "inode num = %d\n", st.ino);
  printf(stdout, "Number of links = %d\n", st.nlink);
  printf(stdout, "Block Addresses:\n");
  if (st.type == T_EXTENT) {
    for (i = 0; i < NDIRECT + 1; i++) {
     //first block pointer 
	  int bloPtr = (st.bloaddrs[i] >> 8); 
	  int len = (st.bloaddrs[i] & 0xff); // length of extent
      printf(stdout, "  Pointer %d's address = %d with size = %d\n", i, bloPtr, len);
    }
  } else {
    for (i = 0; i < NDIRECT + 1; i++) {
      printf(stdout, "  Pointer %d's address = %d\n", i, st.bloaddrs[i]);
    }
  }
  //printf(stdout, "Disk address of block = %d\n", st.bloaddrs);
  printf(stdout, "Number of blocks = %d\n", st.length);
  if (st.size != (512 + 8) ) {
    printf(stdout, "File size is different from expected, size = %d\n", st.size);
    printf(stdout, "File type = %d\n", st.type);
  }

  printf(stdout, "************* Starting test for file2 ***************\n");
  int j;
  for (j = 0; j < 65; j++) {
    if(write(fd2, "hello\n", 7) != 7) {
      printf(stdout, "Error: Writing hello to file2 line%d  failed\n", i);
      exit();
    }
    else{printf(stdout, "Writing hello to line%d  of file2\n", j);}
  }
  
  fstat(fd2, &st2);
  printf(stdout, "File size i= %d\n", st2.size);
  printf(stdout, "File type = %d\n", st2.type);
  printf(stdout, "File system device = %d\n", st2.dev);
  printf(stdout, "inode num = %d\n", st2.ino);
  printf(stdout, "Number of links = %d\n", st2.nlink);
  printf(stdout, "Block Addresses:\n");
  if (st2.type == T_EXTENT) {
    for (j = 0; j < NDIRECT + 1; j++) {
     //first block pointer 
	  int bloPtr2 = (st2.bloaddrs[j] >> 8); 
	  int len2 = (st2.bloaddrs[j] & 0xff); // length of extent
      printf(stdout, " File2: Pointer %d's address = %d with size = %d\n", j, bloPtr2, len2);
    }
  } else {
    for (j = 0; j < NDIRECT + 1; j++) {
      printf(stdout, " File2: Pointer %d's address = %d\n", j, st2.bloaddrs[j]);
    }
  }
  //printf(stdout, "Disk address of block = %d\n", st.bloaddrs);
  printf(stdout, "Number of blocks = %d\n", st2.length);
  if (st2.size != (448 + 7) ) {
    printf(stdout, "File2 size is different from expected, size = %d\n", st2.size);
    printf(stdout, "File2 type = %d\n", st2.type);
  }
  //Lseek testing begins here
  int fd3;
  if(fd3 = open("mytestFile3.txt", O_CREATE | O_RDWR | O_EXTENT) <= 1){
    printf(stdout, "Error: Issue creating file1\n");
  }
  if(write(fd3, "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", 64) != 64){
    printf(stdout, "Error: Writing to file3 failed\n");
    exit();
  }
  printf(stdout, "Testing lseek(). Current file contents:\n");
  
  // output contents of the file
  int n;
  char buf[512];
  while((n = read(fd3, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      printf(1, "cat: write error\n");
      exit();
    }
  }
  if(n < 0)
    printf(1, "cat: read error\n");
  
  // the offset should be 64 before this
  if (lseek(fd3, 26) == -1){
    printf(stdout, "lseek error\n");
  }
  if(write(fd3, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 36) != 36) {
      printf(stdout, "Error: Writing to file3 failed\n");
      exit();
  }
  // should see a gap of 10 zeroes before the last hello\n
  printf(stdout, "Capitals and digits swapped. Current file contents:\n");
  
  // output contents of the file
  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      printf(1, "cat: write error\n");
      exit();
    }
  }
  if(n < 0)
    printf(1, "cat: read error\n");;

  if(lseek(fd, -1) == 0){
    printf(stdout, "Error: lseek() allowed negative offset\n");
  } else{
    printf(stdout, "lseek() correctly didn't allow a negative offset\n");
  }
  if(lseek(fd, 0x7FFFFFFF) == 0){
    printf(stdout, "Error: lseek() allowed offset outside the end of the file\n");
  } else{
    printf(stdout, "lseek() correctly didn't allow offset outside the end of the file\n");
  }

  printf(stdout, "***********Testing Extent File system completed!*************\n");
  close(fd);
  close(fd2);
  close(fd3);
  exit();
}