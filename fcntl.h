#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200
// add an O_EXTENT flag to the open() system call
// that will create an extent based file
// the number 0x201 is arbitrary
#define O_EXTENT  0x201