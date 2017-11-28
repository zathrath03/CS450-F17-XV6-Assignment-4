#define T_DIR  1   // Directory
#define T_FILE 2   // File
#define T_DEV  3   // Device
// create a new type of file to support
// inodes with pointers and length
#define T_EXTENT 4 // Extent based file

struct stat {
  short type;  // Type of file
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short nlink; // Number of links to file
  uint size;   // Size of file in bytes
  // adding an address after discussion with the TA
  // address wasn't included for the default fstat
  uint addrs; // Disk address of block
  // length will be 1 by default for T_FILE /T_DIR / T_DEV types
  // length will be the last byte of the address for T_EXTENT
  uint length; // Number of blocks
};
