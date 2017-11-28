#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200
// add an O_EXTENT flag to the open() system call
// that will create an extent based file
// #define O_EXTENT 0X???
// How should we decide what values to use for ???